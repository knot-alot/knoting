import {
    ClientPlayer,
    GameObject,
    input,
    KeyCode,
    MouseButtonCode,
    Transform,
    RigidBody,
    Vec2,
    Vec3,
    Quat,
    EditorCamera,
    storage, Hierarchy, UUID, scene, network, Particle,
} from "knoting";

import * as math from "./math.js";

export default class PlayerMovement extends GameObject {
    clientPlayer?: ClientPlayer;
    transform?: Transform;
    rigidBody?: RigidBody;
    hierarchy?: Hierarchy;
    particle?: Particle;

    cameraChild?: GameObject;
    maxHealth: number = 1000;
    currentHealth: number = 0;
    bullets: Array<GameObject>;
    is_teamA: boolean;
    fireRate: number = 0.01;
    timePassed: number = 0;

    getCameraChild() {
        let children: Array<UUID> = this.hierarchy.getChildren();

        for (let c in children) {
            let childObj = scene.getGameObjectFromID(children[c]);
            if (!childObj.hasComponent("editorCamera"))
                continue;

            this.cameraChild = childObj;
            break;
        }
    }

    awake() {
        this.clientPlayer = this.getComponent("clientPlayer");
        this.transform = this.getComponent("transform");
        this.rigidBody = this.getComponent("rigidBody");
        this.hierarchy = this.getComponent("hierarchy");
        this.particle = this.getComponent("particles");

        if (!this.clientPlayer) {
            console.error("Failed to acquire ClientPlayer inside Player");
            return;
        }

        let playerNum = this.clientPlayer.getClientNumber();
        this.is_teamA = playerNum % 2 != 0;
    }

    shoot() {
        let forward: Vec3 = math.multiplyConst(this.transform.getForward(), 10.0);
        let spawnPos: Vec3 = math.add(this.transform.getPosition(), forward);
        console.log(`spawnPos = ${spawnPos}`);
        console.log(`position = ${this.transform.getPosition()}`);
        let shooDir: Vec3 = this.getShootDir(spawnPos);

        this.creatBullet(shooDir, spawnPos);
    }

    creatBullet(shootDir: Vec3, spawnPos: Vec3) {
        let bullet = scene.createBullet(this.is_teamA, spawnPos);
        let rigidBody = bullet.getComponent("rigidBody");
        rigidBody.addForce(math.multiplyConst(shootDir, 100.0));
    }

    getShootDir(spawnPos: Vec3): Vec3 {
        let edcam = this.cameraChild.getComponent("editorCamera")
        let a = edcam.getLookTarget();
        let shootDir = math.subtract(a, spawnPos);
        shootDir = math.normalize(shootDir);

        return shootDir;
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

    currentPlayerUpdate() {
        this.playerInputs();

        if (input.keyOnTrigger(KeyCode.U)) {
            let pauseCamera: GameObject = storage.retrieve("pauseCamera");
            let pauseCameraCamera: EditorCamera = pauseCamera.getComponent("editorCamera");
            pauseCameraCamera.setAsActiveCamera();
        }
        if (input.keyOnTrigger(KeyCode.I)) {
            let editorCamera = this.cameraChild.getComponent("editorCamera");
            editorCamera.setAsActiveCamera();
        }

        if (input.mouseButtonPressed(MouseButtonCode.Left)) {
            if (this.timePassed > (.0 / this.fireRate)) {
                console.log("SHOOTING");
                this.removeHealth(1);
                this.shoot()
                this.timePassed = 0;
            }
            let forward: Vec3 = math.multiplyConst(this.transform.getForward(), 1.0);
            let spawnPos: Vec3 = math.add(this.transform.getPosition(), forward);
            let shooDir: Vec3 = this.getShootDir(spawnPos);
            this.particle.setLookAt(math.multiplyConst(shooDir, 4));
            this.particle.setPosition(spawnPos);
            this.particle.setParticlesPerSecond(50);
        } else {
            this.particle.setParticlesPerSecond(0);
        }

        if (input.keyPressed(KeyCode.Enter)) { //AND looking at paint reservoir
            this.addHealth(1);
        }
    }

    update(deltaTime: number) {
        this.timePassed += deltaTime;

        if (!this.cameraChild) {
            this.getCameraChild();
        }

        if (this.clientPlayer.getClientNumber() == network.getClientNumber()) {
            this.currentPlayerUpdate();
        }
    }

    lateUpdate() {
        if (network.isServer()) {
            this.playerRotation();
            this.playerMovement();
        }
    }

    playerInputs() {
        let player_inputs: Vec2 = [0, 0];

        if (input.keyPressed(KeyCode.A)) {
            player_inputs = math.add([0, 1], player_inputs);
        }
        if (input.keyPressed(KeyCode.D)) {
            player_inputs = math.add([0, -1], player_inputs);
        }
        if (input.keyPressed(KeyCode.W)) {
            player_inputs = math.add(player_inputs, [1, 0]);
        }
        if (input.keyPressed(KeyCode.S)) {
            player_inputs = math.add(player_inputs, [-1, 0]);
        }
        this.clientPlayer?.setMoveAxis(player_inputs);

        this.clientPlayer?.setJumpPressed(input.keyOnTrigger(KeyCode.Space));
        this.clientPlayer?.setIsShooting(
            input.mouseButtonPressed(MouseButtonCode.Left)
        );

        // let playerLook: Vec2 = [0, 0];
        // let relMousePosition: Vec2 = input.getRelativePosition();
        // if (relMousePosition[0] > 0) {
        //     playerLook[0] = 1;
        // } else if (relMousePosition[0] < 0) {
        //     playerLook[0] = -1;
        // }
        // if (relMousePosition[1] > 0) {
        //     playerLook[1] = 1;
        // } else if (relMousePosition[1] < 0) {
        //     playerLook[1] = -1;
        // }
        let childCamera: EditorCamera = this.cameraChild.getComponent("editorCamera");
        let rotation = childCamera.getRotationEuler();

        rotation[0] = 0.0;
        rotation[1] = -rotation[1] + 90.0;
        rotation[2] = 0.0;
        this.transform.setRotationEuler(rotation);
        this.rigidBody.setRotationEuler(rotation);

        this.clientPlayer?.setLookAxis(rotation);
    }

    playerRotation() {
        if (!network.isServer() && this.clientPlayer.getClientNumber() == network.getClientNumber())
            return;

        let lookTarget: Vec3 = this.clientPlayer.getLookAxis();

        this.transform.setRotationEuler([0, lookTarget[1], 0]);
        this.rigidBody.setRotationEuler([0, lookTarget[1], 0]);
    }

    playerMovement() {
        let moveAxis: Vec2 = this.clientPlayer!.getMoveAxis();

        let playerForward: Vec3 = this.transform!.getForward();
        playerForward = math.multiply(
            [moveAxis[0], moveAxis[0], moveAxis[0]],
            playerForward
        );

        let playerRight: Vec3 = this.transform!.getRight();
        playerRight = math.multiply(
            [moveAxis[1], moveAxis[1], moveAxis[1]],
            playerRight
        );

        let playerDir: Vec3 = math.add(playerForward, playerRight);

        playerDir = math.normalize(playerDir);

        let baseMulti: number = 1000;
        let moveMulti: number = 2;

        baseMulti = baseMulti * moveMulti;

        let force = math.multiply([baseMulti, baseMulti, baseMulti], playerDir);

        if (math.notNaN(force)) {
            this.rigidBody?.addForce(force);
        }
    }
}
