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

    update() {
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

        
    }
}