import {CollisionDetection, ContactData, GameObject, Mesh, RigidBody, Team,} from "knoting";

export default class floor extends GameObject {
    data: Array<ContactData>
    rigidBody?: RigidBody;
    detection?: CollisionDetection;
    mesh?: Mesh;

    update() {
        this.data = this.detection.getContactDataByActor(this.rigidBody.getActor());
        for (let i = 0; i < this.data.length; i++) {
            if (this.data[i].getContactActorName() == "RED") {
                this.mesh.addContactPosition(this.data[i].getContactPoint(), Team.TeamRed);
            } else if (this.data[i].getContactActorName() == "BLUE") {
                this.mesh.addContactPosition(this.data[i].getContactPoint(), Team.TeamBule);
            }
        }
    }
}





