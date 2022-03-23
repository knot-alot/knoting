import {GameObject, Name, scene, storage} from "knoting";

export class Reservoir extends GameObject {
    maxHealth: number = 1000;
    currentHealth: number = 0;

    awake() {
        this.currentHealth = this.maxHealth;
    }

    update() {
        if (this.currentHealth == 0) this.death();
    }

    removeHealth(health: number) {
        this.currentHealth -= health;

        if (this.currentHealth < 0) this.currentHealth = 0;

    }

    death() {
        scene.removeGameObject(this.getID());

        if (this.getComponent("Name").getName() == "orange") storage.store("winner", "blue");
        else storage.store("winner", "orange");

        storage.store("gameOver", true);

    }

}