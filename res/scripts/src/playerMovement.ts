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

    cameraChild?: GameObject;

    maxHealth: number = 100;
    currentHealth: number = 0;

    getCameraChild() {
        let children: Array<UUID> = this.hierarchy.getChildren();

        for (let c in children) {
            let childObj = scene.getGameObjectFromID(children[c]);
            if (!childObj.hasComponent("editorCamera"))
                continue;

            this.cameraChild = childObj;
            break;
        }

        storage.store("isAlive" + this.clientPlayer.getClientNumber(), false);
    }

    awake() {
        this.clientPlayer = this.getComponent("clientPlayer");
        this.transform = this.getComponent("transform");
        this.rigidBody = this.getComponent("rigidBody");
        this.hierarchy = this.getComponent("hierarchy");
        this.currentHealth = this.maxHealth;
    }

    update(deltaTime: number) {
        if (!this.cameraChild) {
            this.getCameraChild();
        }
    }

    lateUpdate() {
        if (!network.isServer()) {
            return;
        }

        this.playerRotation();
        this.playerMovement();

        if (input.keyOnTrigger(KeyCode.U) && this.clientPlayer.getClientNumber() == 0) {
            let clientNumber = this.clientPlayer.getClientNumber();
            storage.store("isAlive" + clientNumber, !storage.retrieve("isAlive" + clientNumber));
        }


        if (this.clientPlayer.getClientNumber() == network.getClientNumber()) {
            this.playerInputs();

            if (!storage.retrieve("isAlive" + this.clientPlayer.getClientNumber())) {
                let pauseCamera: GameObject = storage.retrieve("pauseCamera");
                let pauseCameraCamera: EditorCamera = pauseCamera.getComponent("editorCamera");
                pauseCameraCamera.setAsActiveCamera();

                this.transform.setPosition([0, 100, 0]);
                this.rigidBody.setPosition([0, 100, 0]);

            } else {
                let editorCamera = this.cameraChild.getComponent("editorCamera");
                editorCamera.setAsActiveCamera();
            }
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

        if (this.clientPlayer.getClientNumber() == 0)
            console.log(`rotation: ${rotation}`);

        this.clientPlayer?.setLookAxis(rotation);
    }

    playerRotation() {
        if (!network.isServer() && this.clientPlayer.getClientNumber() == network.getClientNumber())
            return;

        let lookTarget: Vec3 = this.clientPlayer.getLookAxis();

        if (this.clientPlayer.getClientNumber() == 0)
            console.log(`lookTarget: ${lookTarget}`);

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
        let boostMulti: number = 10
        let moveMulti: number = 2;

        baseMulti = baseMulti * moveMulti;
        if (this.clientPlayer.getJumpPressed())
            baseMulti *= boostMulti;

        let force = math.multiply([baseMulti, baseMulti, baseMulti], playerDir);

        if (math.notNaN(force)) {
            this.rigidBody?.addForce(force);
        }
    }
}
