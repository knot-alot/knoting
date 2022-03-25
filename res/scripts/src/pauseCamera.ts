import {GameObject, storage} from "knoting";

export default class PauseCamera extends GameObject {

    awake() {
        storage.store("pauseCamera", this);
    }

}