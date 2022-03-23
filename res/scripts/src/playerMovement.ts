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
    storage,
} from "knoting";

import * as math from "./math.js";

export default class Player_movement extends GameObject {
    clientPlayer?: ClientPlayer;
    transform?: Transform;
    rigidBody?: RigidBody;

    awake() {
        this.clientPlayer = this.getComponent("clientPlayer");
        this.transform = this.getComponent("transform");
        this.rigidBody = this.getComponent("rigidBody");
    }

    update(deltaTime: number) {
        this.playerInputs();
        storage.store("asd", this);
    }

    lateUpdate() {
        this.playerMovement();
        let asd: Player_movement = storage.retrieve("asd");
        let id = asd.getID();
        console.log(`id: ${id}`);
    }

    playerInputs() {
        let player_inputs: Vec2 = [0, 0];

        if (input.keyPressed(KeyCode.A)) {
            player_inputs = math.add(player_inputs, [-1, 0]);
        }
        if (input.keyPressed(KeyCode.D)) {
            player_inputs = math.add(player_inputs, [1, 0]);
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
            input.mouseButtonHeldDown(MouseButtonCode.Left)
        );

        let player_look: Vec2 = [0, 0];
        let relMousePosition: Vec2 = input.getRelativePosition();
        if (relMousePosition[0] > 0) {
            player_look[0] = 1;
        } else if (relMousePosition[0] < 0) {
            player_look[0] = -1;
        }
        if (relMousePosition[1] > 0) {
            player_look[1] = 1;
        } else if (relMousePosition[1] < 0) {
            player_look[1] = -1;
        }
        this.clientPlayer?.setLookAxis(player_look);
    }

    playerMovement() {
        //Camera Rotation X
        let player_look: Vec2 = this.clientPlayer!.getLookAxis();
        let look_x: number = player_look[0];

        let rotDeg: Vec3 = [0, 0, 0];
        let lookMulti: number = 3;
        if (look_x > 0) {
            rotDeg[1] = -1 * lookMulti;
        } else if (look_x < 0) {
            rotDeg[1] = 1 * lookMulti;
        }

        let playerRot: Quat = this.transform!.getRotation();
        let newRot: Vec3 = math.multiplyQuat(rotDeg, playerRot);
        this.transform!.setRotation(newRot);
        this.rigidBody!.setRotation(this.transform!.getRotation());

        let moveAxis: Vec2 = this.clientPlayer!.getMoveAxis();
        let playerForward: Vec3 = this.transform!.getForward();
        playerForward = math.multiply(
            [moveAxis[1], moveAxis[1], moveAxis[1]],
            playerForward
        );

        let playerRight: Vec3 = this.transform!.getRight();
        playerRight = math.multiply(
            [moveAxis[0], moveAxis[0], moveAxis[0]],
            playerRight
        );

        let playerDir: Vec3 = math.add(playerForward, playerRight);

        playerDir = math.normalize(playerDir);

        let baseMulti: number = 1000;
        let moveMulti: number = 2;

        baseMulti = baseMulti * moveMulti;

        let force = math.multiply([baseMulti, baseMulti, baseMulti], playerDir);

        if (math.notNaN(force)) this.rigidBody?.addForce(force);
    }
}
