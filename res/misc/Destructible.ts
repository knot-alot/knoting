import {GameObject} from "knoting";

export class Destructible extends GameObject {
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
        //destroy object
    }

}