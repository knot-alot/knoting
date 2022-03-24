import {CollisionDetection, ContactData, GameObject, RigidBody,scene} from "knoting";

export default class bullet extends GameObject {
    data: Array<ContactData>
    rigidBody?: RigidBody;
    detection?: CollisionDetection;

    lateUpdate() {
        this.data = this.detection.getContactDataByActor(this.rigidBody.getActor());
        if(!(this.data.length == 0)){
            scene.removeGameObject(this.getID());
        }
    }
}