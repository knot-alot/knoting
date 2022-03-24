import {
    GameObject,
    input,
    KeyCode,
    MouseButtonCode,
    RigidBody,
    scene,
    Particle,
    Hierarchy,
    Vec3,
    Transform,
} from "knoting";

import * as math from "./math.js";

export default class Player extends GameObject {
    maxHealth: number = 1000;
    currentHealth: number = 0;
    bullets: Array<GameObject>;
    is_teamA: boolean;
    timePassed: number;
    fireRate: number = 0.1;
    transform?: Transform;
    Hierarchy?: Hierarchy;
    Particle?: Particle;


    awake() {
        this.currentHealth = this.maxHealth;
    }

    update() {
        if (input.mouseButtonPressed(MouseButtonCode.Left)) {
            this.removeHealth(1);
            this.shoot()
            //shoot
        }else{
            this.Particle.setParticlesPerSecond(0);
        }

        if (input.keyPressed(KeyCode.Enter)) { //AND looking at paint reservoir
            this.addHealth(1);
        }

    }

    lateUpdate() {
        this.bullets.forEach(this.deleteBullet);

    }

    shoot() {
        let spawnPos: Vec3 = math.add(this.transform.getPosition(), this.transform.getForward());
        let shooDir: Vec3 = this.getShootDir(spawnPos);
        this.Particle.setLookat(math.multiplyConst(shooDir, 4));
        this.Particle.setPosition(spawnPos);
        this.Particle.setParticlesPerSecond(50);
        this.creatBullet(shooDir, spawnPos);
    }

    creatBullet(shootDir: Vec3, spawnPos: Vec3) {

        let bullet = scene.createBullet(this.is_teamA, spawnPos);
        this.bullets.push(bullet);
        let rigidBody = bullet.getComponent("rigidBody");
        rigidBody.addForce(math.multiplyConst(shootDir, 100.0));
    }

    deleteBullet(bullet) {
        let detection = bullet.getComponent("CollisionDetection");
        let rigiBody = bullet.getComponent("rigidBody");
        if (!detection.getContactDataByActor(
            rigiBody.getActor().getID()).empty()) {
            detection.removeSearchActor(rigiBody.getActor().getID());
            scene.removeGameObject(bullet.getID());
        }
    }

    getShootDir(spawnPos: Vec3): Vec3 {
        if (this.Hierarchy.hasChildren()) {
            let len = this.Hierarchy.getChildren().length;
            for (let i = 0; i < len; i++) {
                let cam = scene.getGameObjectFromID(this.Hierarchy.getChildren()[i]);
                if (!cam || !cam.hasComponent("editorCamera")) {
                    continue;
                }
                let edcam = cam.getComponent("editorCamera")
                let a = edcam.getLookTarget();
                let shootDir = math.subtract(a, spawnPos);
                shootDir = math.normalize(shootDir);

                return shootDir;
            }
        }

        return
    }

    removeHealth(health: number) {
        this.currentHealth -= health;

        if (this.currentHealth < 0) this.currentHealth = 0;

    }

    addHealth(health: number) {
        this.currentHealth += health;

        if (this.currentHealth > this.maxHealth) this.currentHealth = this.maxHealth;
    }

    death() {
        //respawn stuff
    }


}




