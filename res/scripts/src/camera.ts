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
    storage,
} from "knoting";

import * as math from "./math.js";

export default class Camera extends GameObject {
    transform?: Transform;
    rigidBody?: RigidBody;

    awake() {
        this.transform = this.getComponent("transform");
        this.rigidBody = this.getComponent("rigidBody");
    }

    update(deltaTime: number) {

    }

    lateUpdate() {

    }

}