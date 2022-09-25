package window.input;

import static org.lwjgl.glfw.GLFW.*;

public class Keyboard {
    private final long window;
    
    // Letters
    public Key q = new Key();
    public Key w = new Key();
    public Key e = new Key();
    public Key r = new Key();
    public Key t = new Key();
    public Key z = new Key();
    public Key u = new Key();
    public Key i = new Key();
    public Key o = new Key();
    public Key p = new Key();
    public Key a = new Key();
    public Key s = new Key();
    public Key d = new Key();
    public Key f = new Key();
    public Key g = new Key();
    public Key h = new Key();
    public Key j = new Key();
    public Key k = new Key();
    public Key l = new Key();
    public Key y = new Key();
    public Key x = new Key();
    public Key c = new Key();
    public Key v = new Key();
    public Key b = new Key();
    public Key n = new Key();
    public Key m = new Key();

    // Numbers
    public Key num0 = new Key();
    public Key num1 = new Key();
    public Key num2 = new Key();
    public Key num3 = new Key();
    public Key num4 = new Key();
    public Key num5 = new Key();
    public Key num6 = new Key();
    public Key num7 = new Key();
    public Key num8 = new Key();
    public Key num9 = new Key();

    // Signs
    public Key period = new Key();
    public Key comma = new Key();
    public Key plus = new Key();
    public Key minus = new Key();

    // Special
    public Key esc = new Key();
    public Key tab = new Key();
    public Key caps = new Key();
    public Key shift = new Key();
    public Key ctrl = new Key();
    public Key alt = new Key();
    public Key space = new Key();
    public Key enter = new Key();
    public Key insert = new Key();
    public Key delete = new Key();
    public Key up = new Key();
    public Key left = new Key();
    public Key down = new Key();
    public Key right = new Key();

    // Function
    public Key f1 = new Key();
    public Key f2 = new Key();
    public Key f3 = new Key();
    public Key f4 = new Key();
    public Key f5 = new Key();
    public Key f6 = new Key();
    public Key f7 = new Key();
    public Key f8 = new Key();
    public Key f9 = new Key();
    public Key f10 = new Key();
    public Key f11 = new Key();
    public Key f12 = new Key();

    public Keyboard(long window) {
        this.window = window;
    }
    
    public void update() {
        // Letters
        q.update(Key.getKeyState(window, GLFW_KEY_Q));
        w.update(Key.getKeyState(window, GLFW_KEY_W));
        e.update(Key.getKeyState(window, GLFW_KEY_E));
        r.update(Key.getKeyState(window, GLFW_KEY_R));
        t.update(Key.getKeyState(window, GLFW_KEY_T));
        z.update(Key.getKeyState(window, GLFW_KEY_Z));
        u.update(Key.getKeyState(window, GLFW_KEY_U));
        i.update(Key.getKeyState(window, GLFW_KEY_I));
        o.update(Key.getKeyState(window, GLFW_KEY_O));
        p.update(Key.getKeyState(window, GLFW_KEY_P));
        a.update(Key.getKeyState(window, GLFW_KEY_A));
        s.update(Key.getKeyState(window, GLFW_KEY_S));
        d.update(Key.getKeyState(window, GLFW_KEY_D));
        f.update(Key.getKeyState(window, GLFW_KEY_F));
        g.update(Key.getKeyState(window, GLFW_KEY_G));
        h.update(Key.getKeyState(window, GLFW_KEY_H));
        j.update(Key.getKeyState(window, GLFW_KEY_J));
        k.update(Key.getKeyState(window, GLFW_KEY_K));
        l.update(Key.getKeyState(window, GLFW_KEY_L));
        y.update(Key.getKeyState(window, GLFW_KEY_Y));
        x.update(Key.getKeyState(window, GLFW_KEY_X));
        c.update(Key.getKeyState(window, GLFW_KEY_C));
        v.update(Key.getKeyState(window, GLFW_KEY_V));
        b.update(Key.getKeyState(window, GLFW_KEY_B));
        n.update(Key.getKeyState(window, GLFW_KEY_N));
        m.update(Key.getKeyState(window, GLFW_KEY_M));

        // Numbers
        num0.update(Key.getKeyState(window, GLFW_KEY_0));
        num1.update(Key.getKeyState(window, GLFW_KEY_1));
        num2.update(Key.getKeyState(window, GLFW_KEY_2));
        num3.update(Key.getKeyState(window, GLFW_KEY_3));
        num4.update(Key.getKeyState(window, GLFW_KEY_4));
        num5.update(Key.getKeyState(window, GLFW_KEY_5));
        num6.update(Key.getKeyState(window, GLFW_KEY_6));
        num7.update(Key.getKeyState(window, GLFW_KEY_7));
        num8.update(Key.getKeyState(window, GLFW_KEY_8));
        num9.update(Key.getKeyState(window, GLFW_KEY_9));

        // Signs
        period.update(Key.getKeyState(window, GLFW_KEY_PERIOD));
        comma.update(Key.getKeyState(window, GLFW_KEY_COMMA));
        plus.update(Key.getKeyState(window, GLFW_KEY_EQUAL));
        minus.update(Key.getKeyState(window, GLFW_KEY_MINUS));

        // Special
        esc.update(Key.getKeyState(window, GLFW_KEY_ESCAPE));
        tab.update(Key.getKeyState(window, GLFW_KEY_TAB));
        caps.update(Key.getKeyState(window, GLFW_KEY_CAPS_LOCK));
        shift.update(Key.getKeyState(window, GLFW_KEY_LEFT_SHIFT));
        ctrl.update(Key.getKeyState(window, GLFW_KEY_LEFT_CONTROL));
        alt.update(Key.getKeyState(window, GLFW_KEY_LEFT_ALT));
        space.update(Key.getKeyState(window, GLFW_KEY_SPACE));
        enter.update(Key.getKeyState(window, GLFW_KEY_ENTER));
        insert.update(Key.getKeyState(window, GLFW_KEY_INSERT));
        delete.update(Key.getKeyState(window, GLFW_KEY_DELETE));
        up.update(Key.getKeyState(window, GLFW_KEY_UP));
        left.update(Key.getKeyState(window, GLFW_KEY_LEFT));
        down.update(Key.getKeyState(window, GLFW_KEY_DOWN));
        right.update(Key.getKeyState(window, GLFW_KEY_RIGHT));

        // Function
        f1.update(Key.getKeyState(window, GLFW_KEY_F1));
        f2.update(Key.getKeyState(window, GLFW_KEY_F2));
        f3.update(Key.getKeyState(window, GLFW_KEY_F3));
        f4.update(Key.getKeyState(window, GLFW_KEY_F4));
        f5.update(Key.getKeyState(window, GLFW_KEY_F5));
        f6.update(Key.getKeyState(window, GLFW_KEY_F6));
        f7.update(Key.getKeyState(window, GLFW_KEY_F7));
        f8.update(Key.getKeyState(window, GLFW_KEY_F8));
        f9.update(Key.getKeyState(window, GLFW_KEY_F9));
        f10.update(Key.getKeyState(window, GLFW_KEY_F10));
        f11.update(Key.getKeyState(window, GLFW_KEY_F11));
        f12.update(Key.getKeyState(window, GLFW_KEY_F12));
    }
}
