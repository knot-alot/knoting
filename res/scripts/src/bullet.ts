import {CollisionDetection, ContactData, GameObject, RigidBody, scene} from "knoting";

export default class bullet extends GameObject {
    data: Array<ContactData>

    rigidBody?: RigidBody;
    detection?: CollisionDetection;

    awake() {
        this.rigidBody = this.getComponent("rigidBody");
        this.detection = this.getComponent("collisionDetection");
    }

    lateUpdate() {
        this.data = this.detection.getContactDataByActor(this.rigidBody.getActor());
        if (!(this.data.length == 0)) {
            this.detection.removeSearchActor(this.rigidBody.getActor());
            scene.removeGameObject(this.getID());
        }
    }
}