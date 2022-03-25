import {
    ClientPlayer,
    CollisionDetection,
    ContactData,
    GameObject,
    Mesh, Name,
    Raycast,
    RigidBody,
    scene, storage,
    Team,
    Transform,
    Vec3
} from "knoting";

export default class bullet extends GameObject {
    data: Array<ContactData>

    rigidBody?: RigidBody;
    detection?: CollisionDetection;
    raycast?: Raycast;
    transform?: Transform;

    awake() {
        this.rigidBody = this.getComponent("rigidBody");
        this.detection = this.getComponent("collisionDetection");
        this.transform = this.getComponent("transform");
        this.raycast = this.getComponent("raycast");
    }

    ray(direction: Vec3) {
        let pos = this.transform.getPosition();
        this.raycast.setOrigin(pos);

        this.raycast.setUnitDir(direction);
        this.raycast.setMaxDistance(100);

        this.raycast.raycast();

        if (!this.raycast.isHit()) {
            console.log("Not a hit");
            return;
        }

        let other = this.raycast.getHitActor();
        scene.removeGameObject(other.getID());
    }

    update() {
        this.data = this.detection.getContactDataByActor(this.rigidBody.getActor());
        if (!(this.data.length > 0)) {
            return;
        }

        let other = this.data[0];
        let contactPoint = other.getContactPoint();
        console.log(`${contactPoint}`);

        this.detection.removeSearchActor(this.rigidBody.getActor());
        let id = other.getContactActor();
        console.log(`Other id: ${id}`);

        let otherObj = scene.getGameObjectFromID(id);
        if (!otherObj) {
            scene.removeGameObject(this.getID());
            return;
        }
        let mesh: Mesh = otherObj.getComponent("mesh");
        if (!mesh) {
            scene.removeGameObject(this.getID());
            return;
        }

        let name: string = this.rigidBody.getName();
        let otherName: Name = otherObj.getComponent("name");
        console.log(`Other's name ${otherName}`);
        let isPlayer: boolean = otherName.getName().startsWith("player");

        if (isPlayer) {
            console.log("IT WAS A PLAYER BABY!");
            let otherClientPlayer: ClientPlayer = otherObj.getComponent("clientPlayer");
            let playerToken: string = "health" + otherClientPlayer.getClientNumber();
            let otherHealth: number = storage.retrieve(playerToken);
            otherHealth -= 10;
            if (otherHealth < 0)
                otherHealth = 0;
            storage.store(playerToken, otherHealth);
            console.log(`new health: ${otherHealth}`);
        } else {
            mesh.addContactPosition(contactPoint, name == "BLUE" ? Team.Blue : Team.Red);
        }

        scene.removeGameObject(this.getID());
    }
}