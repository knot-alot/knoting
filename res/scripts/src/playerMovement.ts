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
    storage, Hierarchy, UUID, scene, network,
} from "knoting";

import * as math from "./math.js";

export default class PlayerMovement extends GameObject {
    clientPlayer?: ClientPlayer;
    transform?: Transform;
    rigidBody?: RigidBody;
    hierarchy?: Hierarchy;

    maxHealth: number = 100;

    cameraChild?: GameObject;
    is_teamA: boolean;

    fireRate: number = 6;
    timePassed: number = 0;
    timeToRespawn: number = 5;
    currentTimeRespawn: number = 0;
    deltaTime: number = 0;

    getCameraChild() {
        let children: Array<UUID> = this.hierarchy.getChildren();

        for (let c in children) {
            let childObj = scene.getGameObjectFromID(children[c]);
            if (!childObj.hasComponent("editorCamera"))
                continue;

            this.cameraChild = childObj;
            break;
        }

        storage.store("health" + this.clientPlayer.getClientNumber(), this.maxHealth);
    }

    awake() {
        this.clientPlayer = this.getComponent("clientPlayer");
        this.transform = this.getComponent("transform");
        this.rigidBody = this.getComponent("rigidBody");
        this.hierarchy = this.getComponent("hierarchy");

        if (!this.clientPlayer) {
            console.error("Failed to acquire ClientPlayer inside Player");
            return;
        }

        let playerNum = this.clientPlayer.getClientNumber();
        this.is_teamA = (playerNum % 2 != 0);

        storage.store("spawnUsage", 0);
    }

    shoot() {
        let forward: Vec3 = math.multiplyConst(this.transform.getForward(), 3.0);
        let spawnPos: Vec3 = math.add(this.transform.getPosition(), forward);
        console.log(`spawnPos = ${spawnPos}`);
        console.log(`position = ${this.transform.getPosition()}`);
        let shooDir: Vec3 = this.getShootDir(spawnPos);
        console.log(`shootDir = ${shooDir}`);

        this.createBullet(shooDir, spawnPos);
    }

    createBullet(shootDir: Vec3, spawnPos: Vec3) {
        let bullet = scene.createBullet(this.is_teamA, spawnPos);
        let rigidBody = bullet.getComponent("rigidBody");
        let forward = math.multiplyConst(this.transform.getForward(), 20.0);
        let arc = math.multiplyConst(shootDir, 5.00);
        rigidBody.addForce(math.add(arc, forward));
    }

    getShootDir(spawnPos: Vec3): Vec3 {
        let edcam = this.cameraChild.getComponent("editorCamera");
        let lookTarg: Vec3 = edcam.getLookTarget();

        let b: Vec3 = math.normalize(lookTarg);
        return b;
    }

    removeHealth(health: number) {
        let currentHealth = storage.retrieve("health" + this.clientPlayer.getClientNumber());
        currentHealth -= health;

        if (currentHealth < 0) currentHealth = 0;

        storage.store("health" + this.clientPlayer.getClientNumber(), currentHealth);
    }

    addHealth(health: number) {
        let currentHealth = storage.retrieve("health" + this.clientPlayer.getClientNumber());
        currentHealth += health;

        if (currentHealth > this.maxHealth) currentHealth = this.maxHealth;

        storage.store("health" + this.clientPlayer.getClientNumber(), currentHealth);
    }

    death() {
        this.transform.setPosition([0, 100, 0]);
        this.rigidBody.setPosition([0, 100, 0]);

        if (this.currentTimeRespawn > 0) {
            if (this.currentTimeRespawn - this.deltaTime <= 0) {
                this.respawn();
                this.currentTimeRespawn = 0;
                console.log("respawning");
            } else {
                this.currentTimeRespawn -= this.deltaTime;
                console.log("reducing the time to respawn " + this.currentTimeRespawn);
            }
        } else {
            console.log("setting the timeToRespawn");
            this.currentTimeRespawn = this.timeToRespawn;
        }
    }

    respawn() {
        storage.store("spawnUsage", storage.retrieve("spawnUsage") + 1);
        storage.store("health" + this.clientPlayer.getClientNumber(), this.maxHealth);
        let rnd: number = storage.retrieve("spawnUsage");
        rnd = rnd % 3;
        let spawns: Array<Vec3> = [];
        if (this.clientPlayer.getClientNumber() % 2 == 0) {
            spawns[0] = storage.retrieve("blue_spawn1");
            spawns[1] = storage.retrieve("blue_spawn2");
            spawns[2] = storage.retrieve("blue_spawn3");
        } else {
            spawns[0] = storage.retrieve("red_spawn1");
            spawns[1] = storage.retrieve("red_spawn2");
            spawns[2] = storage.retrieve("red_spawn3");
        }
        let spawn = spawns[rnd];

        this.transform.setPosition(spawn);
        this.rigidBody.setPosition(spawn);
    }

    currentPlayerUpdate() {
        this.playerInputs();

        // if (input.keyOnTrigger(KeyCode.U)) {
        //     let clientNumber = this.clientPlayer.getClientNumber();
        //     storage.store("health" + clientNumber, storage.retrieve("health" + clientNumber) == this.maxHealth ? 0 : this.maxHealth);
        // }
        // if (input.keyOnTrigger(KeyCode.I)) {
        //     this.respawn();
        // }
    }

    update(deltaTime: number) {
        this.deltaTime = deltaTime;
        this.timePassed += deltaTime;

        if (!this.cameraChild) {
            this.getCameraChild();
        }
    }

    lateUpdate() {
        if (this.clientPlayer.getClientNumber() == network.getClientNumber()) {
            this.currentPlayerUpdate();
        }


        if (!network.isServer()) {
            return;
        }

        this.playerRotation();
        this.playerMovement();

        if (this.clientPlayer.getIsShooting()) {
            if (this.timePassed > (1.0 / this.fireRate)) {
                console.log("SHOOTING");
                this.removeHealth(3);
                this.shoot()
                this.timePassed = 0;
            }
            let forward: Vec3 = math.multiplyConst(this.transform.getForward(), 1.0);
            let spawnPos: Vec3 = math.add(this.transform.getPosition(), forward);
            let shooDir: Vec3 = this.getShootDir(spawnPos);
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
        let currentHealth = storage.retrieve("health" + this.clientPlayer.getClientNumber());
        if (currentHealth <= 0) {
            this.death();
            return;
        }

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
        let boostMulti: number = 10
        let moveMulti: number = 2;

        baseMulti = baseMulti * moveMulti;
        if (this.clientPlayer.getJumpPressed())
            baseMulti *= boostMulti;

        let force = math.multiplyConst(playerDir, baseMulti);

        if (math.notNaN(force)) {
            this.rigidBody?.addForce(force);
        }
    }
}
