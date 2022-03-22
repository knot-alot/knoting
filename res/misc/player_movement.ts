import {
    ClientPlayer,
    GameObject,
    input,
    KeyCode,
    MouseButtonCode,
    Vec2,
    Transform,
    Vec3,
    Quat,
    RigidBody
} from "knoting";
import {math} from "./math.js";

export default class Player_movement extends GameObject {


    awake() {
    }

    update(deltaTime: number) {
        this.playerInputs();
    }

    lateUpdate() {
        this.playerMovement();
    }

    playerInputs() {
        let clientPlayer: ClientPlayer = this.getComponent("clientPlayer");

        let player_inputs: Vec2 = [0, 0];

        if (input.keyPressed(KeyCode.A)) {
            player_inputs = math.add(player_inputs, [1, 0]);
        }
        if (input.keyPressed(KeyCode.D)) {
            player_inputs = math.add(player_inputs, [-1, 0]);
        }
        if (input.keyPressed(KeyCode.W)) {
            player_inputs = math.add(player_inputs, [0, 1]);
        }
        if (input.keyPressed(KeyCode.S)) {
            player_inputs = math.add(player_inputs, [0, -1]);
        }
        clientPlayer.SetMoveAxis(player_inputs);

        clientPlayer.SetJumpPressed(input.keyOnTrigger(KeyCode.Space));
        clientPlayer.SetIsShooting(input.mouseButtonHeldDown(MouseButtonCode.Left));

        let player_look: Vec2 = [0, 0];
        let relMousePosition: Vec2 = input.GetRelativePosition();
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
        clientPlayer.SetLookAxis(player_look);
    }

    playerMovement() {
        let clientPlayer: ClientPlayer = this.getComponent("clientPlayer");
        let transform: Transform = this.getComponent("transform");
        let rigidBody: RigidBody = this.getComponent("rigidBody");

        //Camera Rotation X
        let player_look: Vec2 = clientPlayer.GetLookAxis();
        let look_x: number = player_look[0];

        let rotDeg: Vec3 = [0, 0, 0];
        let lookMulti: number = 3;
        if (look_x > 0) {
            rotDeg[1] = -1 * lookMulti;
        } else if (look_x < 0) {
            rotDeg[1] = 1 * lookMulti;
        }

        let playerRot: Vec3 = transform.getRotationEuler();
        let newRot: Vec3 = math.add(playerRot, rotDeg);
        transform.setRotation(newRot);
        rigidBody.setRotation(transform.getRotation());

        let moveAxis: Vec2 = clientPlayer.GetMoveAxis();
        let playerForward: Vec3 = transform.getForward();
        playerForward = math.multiply([moveAxis[1], moveAxis[1], moveAxis[1]], playerForward);

        let playerRight: Vec3 = transform.getRight();
        playerRight = math.multiply([moveAxis[0], moveAxis[0], moveAxis[0]], playerRight);

        let playerDir: Vec3 = math.add(playerForward, playerRight);

        playerDir = math._normalize(playerDir);

        let baseMulti: number = 1000;
        let moveMulti: number = 2;

        baseMulti = baseMulti * moveMulti;

        rigidBody.addForce(math.multiply([baseMulti, baseMulti, baseMulti], playerDir));
    }
}