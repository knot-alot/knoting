import {CollisionDetection, ContactData, GameObject, Mesh, RigidBody, Team, UUID} from "knoting";

export default class floor extends GameObject {
    data: Array<ContactData>

    rigidBody?: RigidBody;
    detection?: CollisionDetection;
    mesh?: Mesh;

    awake() {
        this.rigidBody = this.getComponent("rigidBody");
        this.detection = this.getComponent("collisionDetection");
        this.mesh = this.getComponent("instanceMesh");
    }

    update() {
        // this.data = this.detection.getContactDataByActor(this.rigidBody.getActor());
        // for (let i = 0; i < this.data.length; i++) {
        //     // if (this.data[i].getContactActorName() == "RED") {
        //     //     console.log(`${this.data[i].getContactActorName()}`);
        //     //     this.mesh.addContactPosition(this.data[i].getContactPoint(), Team.Red);
        //     // } else if (this.data[i].getContactActorName() == "BLUE") {
        //     //     console.log(`${this.data[i].getContactActorName()}`);
        //     //     this.mesh.addContactPosition(this.data[i].getContactPoint(), Team.Blue);
        //     // }
        // }
    }
}





