declare module knoting {
    class Transform {
        position: Array<number>;    //vec3
        rotation: Array<number>;    //vec3
            scale: Array<number>;   //quaternion

        constructor(position: Array<number>, scale: Array<number>, rotation: Array<number>);
        }

        class RigidBody {
        get_velocity(): number;
        set_velocity(velocity:number): void;
        }

        class UUIDGenerator{
        constructor();
        generate(): string;
            //std::unique_ptr<uuids::uuid_random_generator> m_uuidGenerator;
            // std::mt19937 m_randomGenerator;
        }

        class HasOnAwake{
        one: string;
        two: string;

        //enum { value = sizeof(test<T>(0)) == sizeof(char) };
        }

        class HasOnDestroy{
            one: string;
            two: string;

            //enum { value = sizeof(test<T>(0)) == sizeof(char) };
        }

        class GameObject{
            GameObject(handle:number, scene :Scene);
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
            get_game_object_from_component(any):GameObject;
            remove_component(): void;
            //serialize?

            m_handle: number;
            m_scene: Scene;
            //UUIDGenerator: inline static
        }

    class Hierarchy{
        constructor();
        constructor(parent: GameObject, children: Array<number>);
        constructor(parent: GameObject);
        constructor(children: Array<number>);

        get_parent: string;
        has_parent: boolean;
        set_parent(parent: GameObject): void;

        has_children: boolean;
        has_child(string): boolean;
        get_children(): Array <string>;
        add_child(child: GameObject): void;
        remove_child(child: GameObject): void;
        //serialize?

        m_parent: string;
        m_children: Array <string>;
    }

    class name{
        name: string;
        constructor(name: string);
        constructor();

        //inline bool operator==(const Name& other) const { return name == other.name; }
        //inline bool operator!=(const Name& other) const { return name != other.name; }

        //serialize?
    }

    class tag{
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
    }

    class AudioListener{
        get_position(): Array<number>;
        get_rotation(): Array<number>;
        get_forward(): Array<number>;
        get_up(): Array<number>;

        working: boolean;
    }

    class AudioSource{
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

    class EditorCamera{
        constructor();
        get_look_target: Array<number>
    }

    enum MouseButtonCode {
        Left ,
        Right,
        Middle,
        Button3,
        Button4,
        Button5,
        Button6,
        Button7,
        Button8,
        Last,
    };
    enum  JoyStickCode { LeftX, LeftY, RightX, RightY, LeftBumper, RightBumper, Last };
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
    };
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
    };
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
        Enter ,
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
    };

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

