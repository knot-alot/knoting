declare module "knoting" {
    type float = number;
    type Vec2 = Array<number>;
    type Vec3 = Array<number>;
    type Quat = Array<number>;
    type UUID = string;

    class Storage {
        store(key: string, obj: any): void;

        retrieve(key: string): any;
    }

    class Transform {
        setPosition(position: Vec3): void;

        setScale(scale: Vec3): void;

        setRotation(rotation: Quat): void;

        setRotationEuler(euler: Vec3): void;

        getPosition(): Vec3;

        getScale(): Vec3;

        getRotation(): Quat;

        getRotationEuler(): Vec3;

        getForward(): Vec3;

        getUp(): Vec3;

        getRight(): Vec3;
    }

    class RigidBody {
        getActor(): UUID;

        getPosition(): Vec3;

        getRotation(): Quat;

        getRotationEuler(): Vec3;

        setPosition(position: Vec3): void;

        setRotation(rotation: Quat): void;

        setRotationEuler(rotation: Vec3): void;

        getName(): string;

        setName(name: string): void;

        getMass(): number;

        isDynamic(): boolean;

        isKinematic(): boolean;

        getAngularDamping(): number;

        getLinearDamping(): number;

        getAngularVelocity(): Vec3;

        getLinearVelocity(): Vec3;

        setKinematic(toggle: boolean): void;

        setMass(mass: number): void;

        setAngularDamping(damping: number): void;

        setLinearDamping(damping: number): void;

        setAngularVelocity(velocity: number): void;

        setLinearVelocity(velocity: number): void;

        addForce(force: Vec3): void;

        addTorque(torque: Vec3): void;

        clearForce(): void;

        clearTorque(): void;
    }

    class GameObject {
        getID(): UUID;

        getHandle(): number;

        hasNoComponents(): boolean;

        hasComponent(component: string): boolean;

        getComponent(component: string): any;

        addComponent(component: string, ...args: any): any;

        getComponent(component: string): any;

        removeComponent(component: string): void;
    }

    class Hierarchy {
        getParent(): UUID;

        hasParent(): boolean;

        setParent(parent: UUID): void;

        hasChildren(): boolean;

        hasChild(child: UUID): boolean;

        getChildren(): Array<UUID>;

        addChild(child: UUID): void;

        removeChild(child: UUID): void;
    }

    class Name {
        getName(): string;

        setName(name: string): void;
    }

    class Tag {
        getTag(): string;

        setTag(tag: string): void;

        registerTag(tag: string): void;

        unregisterTag(tag: string): void;

        getRegisteredTags(): Array<string>;
    }

    class Scene {
        createGameObject(name: string): GameObject;

        createBullet(is_teamA: boolean, spawnPos: Vec3);

        removeGameObject(object: UUID): void;

        getGameObjectFromID(id: UUID): GameObject;

        findGameObject(name: string): Array<UUID>;

        loadScene(path: string): void;
    }

    class AudioSource {
        isPlaying(): boolean;

        isPaused(): boolean;

        setLoops(loops: boolean): void;

        getLoops(): boolean;

        toggle(): void;

        stop(): void;

        play(): void;
    }

    class EditorCamera {
        setRotationEuler(rotation: Vec3): void;

        getRotationEuler(): Vec3;

        getUP(): Vec3;

        setLookTarget(target: Vec3): void;

        setFov(fov: number): void;

        setZNear(zNear: number): void;

        setZFar(zFar: number): void;

        getLookTarget(target: Vec3): void;

        getFov(fov: number): void;

        getZNear(zNear: number): void;

        getZFar(zFar: number): void;

        setMoveSpeed(speed: number): void;

        getMoveSpeed(): number;

        setAsActiveCamera(): void;
    }

    class Raycast {
        getOrigin(): Vec3;

        getUnitDir(): Vec3;

        getMaxDistance(): number;

        isHit(): boolean;

        getHitPosition(): Vec3;

        getHitNormal(): Vec3;

        getHitDistance(): number;

        setOrigin(origin: Vec3): void;

        setUnitDir(unitDir: Vec3): void;

        setMaxDistance(maxDistance: number): void;

        setFlags()

        getHitActor(): GameObject;

        setRaycast(origin: Vec3, unitDir: Vec3, maxDis: number): void;
    }

    enum MouseButtonCode {
        Left,
        Right,
        Middle,
        Button3,
        Button4,
        Button5,
        Button6,
        Button7,
        Button8,
        Last,
    }

    enum JoyStickCode {
        LeftX,
        LeftY,
        RightX,
        RightY,
        LeftBumper,
        RightBumper,
        Last,
    }

    enum PadCode {
        Pad0,
        Pad1,
        Pad2,
        Pad3,
        Pad4,
        Pad5,
        Pad6,
        Pad7,
        Pad8,
        Pad9,
        Pad10,
        Pad11,
        Pad12,
        Pad13,
        Pad14,
        Pad15,
        Last,
    }

    enum PadButtonCode {
        A,
        B,
        X,
        Y,
        LeftBumper,
        RightBumper,
        Back,
        Start,
        Guide,
        LeftThumb,
        RightThumb,
        DPadUp,
        DPadRight,
        DPadDown,
        DPadLeft,
        Last,
    }

    enum KeyCode {
        Unknown,
        Space,
        Apostrophe,
        Comma,
        Minus,
        Period,
        Slash,
        N0,
        N1,
        N2,
        N3,
        N4,
        N5,
        N6,
        N7,
        N8,
        N9,
        Semicolon,
        Equal1,
        A,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
        I,
        J,
        K,
        L,
        M,
        N,
        O,
        P,
        Q,
        R,
        S,
        T,
        U,
        V,
        W,
        X,
        Y,
        Z,
        LeftBracket,
        Backslash,
        RightBracket,
        GraveAccent,
        World1,
        World2,
        Escape,
        Enter,
        Tab,
        Backspace,
        Insert,
        Delete,
        Right,
        Left,
        Down,
        Up,
        PageUp,
        PageDown,
        Home,
        End,
        CapsLock,
        ScrollLock,
        NumLock,
        PrintScreen,
        Pause,
        F1,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        F10,
        F11,
        F12,
        F13,
        F14,
        F15,
        F16,
        F17,
        F18,
        F19,
        F20,
        F21,
        F22,
        F23,
        F24,
        F25,
        KP0,
        KP1,
        KP2,
        KP3,
        KP4,
        KP5,
        KP6,
        KP7,
        KP8,
        KP9,
        KPDecimal,
        KPDivide,
        KPMultiply,
        KPSubtract,
        KPAdd,
        KPEnter,
        KPEqual,
        LeftShift,
        LeftControl,
        LeftAlt,
        LeftSuper,
        RightShift,
        RightControl,
        RightAlt,
        RightSuper,
        Menu,
        Last,
    }

    class InputManager {
        constructor();

        // KeyBoard Methods
        keyPressed(key: KeyCode): boolean;

        keyHeldDown(key: KeyCode): boolean;

        keyOnTrigger(key: KeyCode): boolean;

        keyOnRelease(key: KeyCode): boolean;

        // Pad Methods
        padButtonPressed(pad: PadCode, button: PadButtonCode): boolean;

        padButtonHeldDown(pad: PadCode, button: PadButtonCode): boolean;

        padButtonTriggered(pad: PadCode, button: PadButtonCode): boolean;

        padButtonReleased(pad: PadCode, button: PadButtonCode): boolean;

        getPadAxis(pad: PadCode, joy_stick: JoyStickCode): number;

        padPresent(pad: PadCode): boolean;

        // Mouse Methods
        mouseButtonPressed(button: MouseButtonCode): boolean;

        mouseButtonHeldDown(button: MouseButtonCode): boolean;

        mouseButtonTriggered(button: MouseButtonCode): boolean;

        mouseButtonReleased(button: MouseButtonCode): boolean;

        mouseDoubleClicked(button: MouseButtonCode): boolean;

        setDoubleClickLimit(ms: number): void;

        getRelativePosition(): Vec2;

        getAbsolutePosition(): Vec2;

        setRelativePosition(relativePosition: Vec2): void;

        setAbsolutePosition(absolutePosition: Vec2): void;

        wheelMoved(): boolean;
    }

    class ClientPlayer {
        getLookAxis(): Vec3;

        getMoveAxis(): Vec2;

        getJumpPressed(): boolean;

        getIsShooting(): boolean;

        setLookAxis(lookAxis: Vec3): void;

        setMoveAxis(moveAxis: Vec2): void;

        setJumpPressed(jumpingPressed: boolean): boolean;

        setIsShooting(isShooting: boolean): boolean;

        getClientNumber(): number;
    }

    class Network {
        getClientNumber(): number;
        
        isServer(): boolean;
    }

    class Particle {
        setParticlesPerSecond(rate: float): void

        setPosition(position: Vec3): void

        setLookAt(lookat: Vec3): void

        getParticlesPerSecond(): float

        getPosition(): Vec3

        getLookAt(): Vec3
    }

    enum ContactType {
        TouchFound,
        TouchPersists,
    }

    class ContactData {
        getContactPoint(): Vec3

        getContactActor(): UUID

        getContactActorName(): string

        getContactType(): ContactType
    }

    enum Team {
        Red,
        Blue,
    }

    class Mesh {
        addContactPosition(position: Vec3, team: Team): void
    }

    class CollisionDetection {
        getContactDataByActor(id: UUID): Array<ContactData>

        getContactDataByName(name: string): Array<ContactData>

        addSearchActor(id: UUID): void

        addSearchName(name: string): void

        removeSearchActor(id: UUID): void

        removeSearchName(name: string): void
    }

    const input: InputManager;
    const scene: Scene;
    const storage: Storage;
    const network: Network;
}
