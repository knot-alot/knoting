import {EditorCamera, GameObject, input, KeyCode, MouseButtonCode} from "knoting";

export default class Player extends GameObject {
    maxHealth: number = 1000;
    currentHealth: number = 0;

    awake() {
        this.currentHealth = this.maxHealth;

    }

    update() {
        if (input.mouseButtonPressed(MouseButtonCode.Left)) {
            this.removeHealth(1);
            //shoot
        }

        if (input.keyPressed(KeyCode.Enter)) { //AND looking at paint reservoir
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

    }

}




