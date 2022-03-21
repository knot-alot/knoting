declare module knoting {
    class Transform {
        position: Array<number>;    //vec3
        rotation: Array<number>;    //vec3
        scale: Array<number>;   //quaternion

        constructor(position: Array<number>, scale: Array<number>, rotation: Array<number>);
    }

    class RigidBody {
        get_velocity(): number;

        set_velocity(velocity: number): void;
    }

    class UUIDGenerator {
        constructor();

        generate(): string;

        //std::unique_ptr<uuids::uuid_random_generator> m_uuidGenerator;
        // std::mt19937 m_randomGenerator;
    }

    class HasOnAwake {
        one: string;
        two: string;

        //enum { value = sizeof(test<T>(0)) == sizeof(char) };
    }

    class HasOnDestroy {
        one: string;
        two: string;

        //enum { value = sizeof(test<T>(0)) == sizeof(char) };
    }

    class GameObject {
        GameObject(handle: number, scene: Scene);

        // inline bool operator==(const GameObject& other) const { return this->get_id() == other.get_id(); }
        // inline bool operator!=(const GameObject& other) const { return this->get_id() != other.get_id(); }
        get_id(): string;

        get_handle(): number;

        has_no_components(): boolean;

        has_component(component: string): boolean;

        get_component(component: string): any;

        has_any_of(component: Array<string>): boolean;

        add_component(component: string, ...Args): any;

        //try_add_component(component: string, ...Args): any;
        get_component(component: string): any;

        get_game_object_from_component(any): GameObject;

        remove_component(): void;

        //serialize?

        m_handle: number;
        m_scene: Scene;
        //UUIDGenerator: inline static
    }

    class Hierarchy {
        constructor();
        constructor(parent: GameObject, children: Array<number>);
        constructor(parent: GameObject);
        constructor(children: Array<number>);

        get_parent: string;
        has_parent: boolean;

        set_parent(parent: GameObject): void;

        has_children: boolean;

        has_child(string): boolean;

        get_children(): Array<string>;

        add_child(child: GameObject): void;

        remove_child(child: GameObject): void;

        //serialize?

        m_parent: string;
        m_children: Array<string>;
    }

    class name {
        name: string;

        constructor(name: string);
        constructor();

        //inline bool operator==(const Name& other) const { return name == other.name; }
        //inline bool operator!=(const Name& other) const { return name != other.name; }

        //serialize?
    }

    class tag {
        constructor(tag: string);

        get_id(): number;

        get_tag(): string;

        //inline bool operator==(const Tag& other) const { return m_id == other.m_id; }
        //inline bool operator!=(const Tag& other) const { return m_id != other.m_id; }

        register_tag(tag: string): void;

        unregister_tag(tag: string): void;

        get_registered_tags(): Array<string>;

        //serialize?

        m_id: number;
        // inline static
    }

    class Scene {
        instantiate(name: string, transform: Transform): GameObject;

        constructor();

        create_game_object(name: string): GameObject;

        add_game_object(handle: number): GameObject;

        remove_game_object(game_object: GameObject): void;

        get_game_object_from_id(id: string): GameObject;

        get_game_object_from_id(handle: number): GameObject;

        // entt::registry& get_registry();
        // static std::optional<std::reference_wrapper<Scene>> get_active_scene();

        set_active_scene(scene: Scene): void;

        //entt::registry m_registry;
        //std::map<uuid, GameObject> m_uuidGameObjectMap;
        //std::map<entt::entity, GameObject> m_entityGameObjectMap;
        //inline static std::optional<std::reference_wrapper<Scene>> s_activeScene = std::nullopt;
    }

    class AudioListener {
        get_position(): Array<number>;

        get_rotation(): Array<number>;

        get_forward(): Array<number>;

        get_up(): Array<number>;

        working: boolean;
    }

    class AudioSource {
        AudioSource(path: string, loops: boolean);

        //on_awake(): void;
        //on_destroy(): void;
        //generate_default_asset(): void;

        get_position(): Array<number>;

        //get_sound(): m_sound;??
        //Channel m_channel ??

        m_isPaused: boolean;
        m_isPlaying: boolean;

        m_loops: boolean;
        //friend class AudioSubSystem;
        //m_sound: FMOD;

    }

    class AudioSubsystem {
        //on_awake(): void;
        //on_destroy(): void;

        play(source: AudioSource): void;

        toggle(source: AudioSource): void;

        stop(source: AudioSource): void;
        stop(): void;

        update(): void;

        add_sound(sound: AudioSource): void;

        update_source(source: AudioSource): void;

        update_listener(source: AudioListener): void;

        set_loops(source: AudioSource, loops: boolean): void;

        //m_system:System
        m_frequency: number;
        m_priority: number;
        m_minDis: number;
        m_maxDis: number;
        //m_result:FMOD_RESULT;
    }

    class EditorCamera {
        constructor();

        get_look_target: Array<number>
    }

    class Raycast {
        constructor();

        //on_awake(): void;
        //on_destroy(): void;
        raycast(): void;

        get_origin(): Array<number>;

        get_unitDir(): Array<number>;

        get_maxDistance(): number;

        get_is_hit(): boolean;

        get_hit_position(): Array<number>;

        get_hit_normal(): Array<number>;

        get_hit_distance(): number;

        //std::weak_ptr<PxShape_ptr_wrapper> get_hit_shape();

        set_origin(origin: Array<number>): void;

        set_unit_dir(unitDir: Array<number>): void;

        set_max_distance(maxDistance: number): void;

        set_raycast(origin: Array<number>, unitDir: Array<number>, maxDistance: number): void;

        get_position_from_transform(): Array<number>;

        m_origin: Array<number>;
        m_unitDir: Array<number>;
        m_maxDistance: number;
        m_isHit: boolean;
        //PxRaycastBuffer m_hit;

    }

    class Shape {
        constructor();

        //on_awake(): void;
        //on_destroy(): void;
        //on_load(): void;

        // std::weak_ptr<PxMaterial_ptr_wrapper> get_material() { return m_material; }
        // std::weak_ptr<PxShape_ptr_wrapper> get_shape() { return m_shape; }

        set_material(material: any): void;

        set_geometry(geometry: Array<number>): void;

        set_local_rotation(rotation: Array<number>): void;

        create_cube_geometry(halfsize: Array<number>): Array<number>;

        create_sphere_geometry(halfsize: number): Array<number>;

        create_capsule_geometry(rasius: number, halfsize: number): Array<number>;

        //    std::shared_ptr<PxMaterial_ptr_wrapper> get_PxMaterial_from_pxmaterial();

        // std::shared_ptr<PxPhysics_ptr_wrapper> m_physics;
        // std::shared_ptr<PxShape_ptr_wrapper> m_shape;
        // std::shared_ptr<PxMaterial_ptr_wrapper> m_material;

        m_shapeSize: Array<number>;
        //    PxGeometryType::Enum m_shapeType;

    }

    class ClientPlayer {
        constructor();

        //void on_awake();
        //void on_destroy();

        //serialize?

        m_thisClientNum: number;
        m_clientNum: number

        m_lookAxis: Array<number>;
        m_moveAxis: Array<number>;

        m_jumpPressed: boolean;
        m_isShooting: boolean;
    }

    class CameraRotation {
        //CameraRotation(Engine& engine);

        on_update(m_delta_time: number): void;

        on_late_update(): void ;

        //void on_awake() override;
        //void on_destroy() override;

        //Engine& m_engine;
        //std::shared_ptr<InputManager> m_inputManager;

        m_lastMousePosition: Array<number>;
        m_mouseSensitivity: Array<number>;
        m_pitchClamp: Array<number>;
        m_currentMousePos: Array<number>;
        m_mouseDelta: Array<number>;

        m_keyboardDirection: Array<number>;
        m_movementMultiplier: Array<number>;
        m_maxMovementMultiplier: Array<number>;
        m_minMovementMultiplier: Array<number>;
        m_moveSpeed: Array<number>;

        m_pitch: number;
        m_yaw: number;
        m_roll: number;

        m_right: Array<number>;
        m_up: Array<number>;
        m_forward: Array<number>;

        m_ePressed: boolean;
        m_lockState: boolean;

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

    enum JoyStickCode { LeftX, LeftY, RightX, RightY, LeftBumper, RightBumper, Last }

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
        Last
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
        Last
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
        Last
    }

// class InputManager {
//     keyState: Array<boolean>;
//     /// KeyBoard Methods
//     declare function key_pressed(b: boolean):KeyCode ;
//     // {
//     //     return this.keyState[key];
//     //
//     // }
//     bool key_held_down(key: KeyCode);
//     bool key_on_trigger(KeyCode key);
//     bool key_on_release(KeyCode key);
//
//     /// Pad Methods
//     bool pad_button_pressed(PadCode pad, PadButtonCode button);
//     bool pad_button_held_down(PadCode pad, PadButtonCode button);
//     bool pad_button_triggered(PadCode pad, PadButtonCode button);
//     bool pad_button_released(PadCode pad, PadButtonCode button);
//     float get_pad_axis(PadCode pad, JoyStickCode joy_stick);
//     bool pad_present(PadCode pad);
//
//     /// Mouse Methods
//     bool mouse_button_pressed(MouseButtonCode button);
//     bool mouse_button_held_down(MouseButtonCode button);
//     bool mouse_button_triggered(MouseButtonCode button);
//     bool mouse_button_released(MouseButtonCode button);
//     bool mouse_double_clicked(MouseButtonCode button);
//     bool wheel_moved();
//     vec2 get_relative_position();
//     vec2 get_absolute_position();
//     vec2 get_wheel_movement();
//     void set_mouse_sensitivity(float sensitivity);
//     void set_double_click_limit(float ms);
//     friend Window;
//
//     void update_pads(GLFWwindow* glfwWindow);
//     void update_relative_positions();
//
//     /// Events
//     void scroll_event(vec2 offset);
//     void key_event(int key, bool pressed);
//     void mouse_button_event(int button, bool pressed);
//     void mouse_event(double x, double y);
//     void update_holds();
//
//     std::array<bool, static_cast<size_t>(KeyCode::Last)> m_keyBinding;
//     std::array<bool, static_cast<size_t>(KeyCode::Last)> m_holdKeyBindings;
//
//     std::array<bool, static_cast<size_t>(KeyCode::Last)> m_mouseBindings;
//     std::array<bool, static_cast<size_t>(KeyCode::Last)> m_holdMouseBindings;
//
//     std::array<std::array<bool, static_cast<size_t>(KeyCode::Last)>, static_cast<size_t>(PadCode::Last)> m_padBindings;
//     std::array<std::array<bool, static_cast<size_t>(KeyCode::Last)>, static_cast<size_t>(PadCode::Last)>
//     m_holdPadBindings;
//
//     std::array<bool, static_cast<size_t>(JoyStickCode::Last)> m_joyStickBindings[static_cast<size_t>(PadCode::Last)];
//
//     vec2 m_lastScroll;
//     vec2d m_mousePosition;
//     vec2d m_lastMousePosition;
//     vec2d m_relativeMousePosition;
//     float m_sensitivity;
// };
}

