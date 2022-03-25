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

export default class Player_movement extends GameObject {
    clientPlayer?: ClientPlayer;
    transform?: Transform;
    rigidBody?: RigidBody;
    hierarchy?: Hierarchy;

    cameraChild?: GameObject;

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
    }

    update(deltaTime: number) {
        if (!this.cameraChild) {
            this.getCameraChild();
        }

        if (this.clientPlayer.getClientNumber() == network.getClientNumber()) {
            this.playerInputs();
        }
    }

    lateUpdate() {
        if (network.isServer()) {
            this.playerRotation();
            this.playerMovement();
        }

        if (input.keyOnTrigger(KeyCode.U)) {
            let pauseCamera: GameObject = storage.retrieve("pauseCamera");
            let pauseCameraCamera: EditorCamera = pauseCamera.getComponent("editorCamera");
            pauseCameraCamera.setAsActiveCamera();
        }
        if (input.keyOnTrigger(KeyCode.I)) {
            let editorCamera = this.cameraChild.getComponent("editorCamera");
            editorCamera.setAsActiveCamera();
        }
    }

    playerInputs() {
        let player_inputs: Vec2 = [0, 0];

        if (input.keyPressed(KeyCode.A)) {
            player_inputs = math.add([1, 0], player_inputs);
        }
        if (input.keyPressed(KeyCode.D)) {
            player_inputs = math.add([-1, 0], player_inputs);
        }
        if (input.keyPressed(KeyCode.W)) {
            player_inputs = math.add(player_inputs, [0, -1]);
        }
        if (input.keyPressed(KeyCode.S)) {
            player_inputs = math.add(player_inputs, [0, 1]);
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
        rotation[1] = -rotation[1] - 180.0;
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
        let moveMulti: number = 2;

        baseMulti = baseMulti * moveMulti;

        let force = math.multiply([baseMulti, baseMulti, baseMulti], playerDir);

        if (math.notNaN(force)) {
            this.rigidBody?.addForce(force);
        }
    }
}
