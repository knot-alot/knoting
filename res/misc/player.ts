import {GameObject, input, KeyCode, MouseButtonCode, scene, Raycast} from "knoting";

export default class Player extends GameObject {
    maxHealth: number = 1000;
    currentHealth: number = 0;

    awake() {
        this.currentHealth = this.maxHealth;
    }

    update() {

        let hit: Raycast;
        hit.setRaycast(this.getComponent("Transform").getPosition(), this.getComponent("Transform").getForward(), 10);
        
        if (input.mouseButtonPressed(MouseButtonCode.Left)) {
            this.removeHealth(1);
            //shoot
        }

        if (input.keyPressed(KeyCode.E) && hit.getHitActor().getComponent("Tag").getTag() == "reservoir") {
            this.addHealth(1);
        }

    }

    removeHealth(health: number) {
        this.currentHealth -= health;

        if (this.currentHealth < 0) this.currentHealth = 0;

    }

    addHealth(health: number) {
        this.currentHealth += health;

        if (this.currentHealth > this.maxHealth) this.currentHealth = this.maxHealth;
    }

    death() {
        scene.removeGameObject(this.getID());
        //respawn stuff
    }

}




