#include "window/input/keyboard.h"


// Window private
void kl::keyboard::update(const WPARAM type, const bool new_state)
{
    switch (type)
    {
    case 'Q':
        q.update(new_state);
        break;
    case 'W':
        w.update(new_state);
        break;
    case 'E':
        e.update(new_state);
        break;
    case 'R':
        r.update(new_state);
        break;
    case 'T':
        t.update(new_state);
        break;
    case 'Z':
        z.update(new_state);
        break;
    case 'U':
        u.update(new_state);
        break;
    case 'I':
        i.update(new_state);
        break;
    case 'O':
        o.update(new_state);
        break;
    case 'P':
        p.update(new_state);
        break;
    case 'A':
        a.update(new_state);
        break;
    case 'S':
        s.update(new_state);
        break;
    case 'D':
        d.update(new_state);
        break;
    case 'F':
        f.update(new_state);
        break;
    case 'G':
        g.update(new_state);
        break;
    case 'H':
        h.update(new_state);
        break;
    case 'J':
        j.update(new_state);
        break;
    case 'K':
        k.update(new_state);
        break;
    case 'L':
        l.update(new_state);
        break;
    case 'Y':
        y.update(new_state);
        break;
    case 'X':
        x.update(new_state);
        break;
    case 'C':
        c.update(new_state);
        break;
    case 'V':
        v.update(new_state);
        break;
    case 'B':
        b.update(new_state);
        break;
    case 'N':
        n.update(new_state);
        break;
    case 'M':
        m.update(new_state);
        break;

    case '0':
        num0.update(new_state);
        break;
    case '1':
        num1.update(new_state);
        break;
    case '2':
        num2.update(new_state);
        break;
    case '3':
        num3.update(new_state);
        break;
    case '4':
        num4.update(new_state);
        break;
    case '5':
        num5.update(new_state);
        break;
    case '6':
        num6.update(new_state);
        break;
    case '7':
        num7.update(new_state);
        break;
    case '8':
        num8.update(new_state);
        break;
    case '9':
        num9.update(new_state);
        break;

    case VK_OEM_PERIOD:
        period.update(new_state);
        break;
    case VK_OEM_COMMA:
        comma.update(new_state);
        break;
    case VK_OEM_PLUS:
        plus.update(new_state);
        break;
    case VK_OEM_MINUS:
        minus.update(new_state);
        break;

    case VK_ESCAPE:
        esc.update(new_state);
        break;
    case VK_TAB:
        tab.update(new_state);
        break;
    case VK_CAPITAL:
        caps.update(new_state);
        break;
    case VK_SHIFT:
        shift.update(new_state);
        break;
    case VK_CONTROL:
        ctrl.update(new_state);
        break;
    case VK_MENU:
        alt.update(new_state);
        break;
    case VK_SPACE:
        space.update(new_state);
        break;
    case VK_RETURN:
        enter.update(new_state);
        break;
    case VK_INSERT:
        insert.update(new_state);
        break;
    case VK_DELETE:
        delet.update(new_state);
        break;

    case VK_UP:
        up.update(new_state);
        break;
    case VK_LEFT:
        left.update(new_state);
        break;
    case VK_DOWN:
        down.update(new_state);
        break;
    case VK_RIGHT:
        right.update(new_state);
        break;

    case VK_F1:
        f1.update(new_state);
        break;
    case VK_F2:
        f2.update(new_state);
        break;
    case VK_F3:
        f3.update(new_state);
        break;
    case VK_F4:
        f4.update(new_state);
        break;
    case VK_F5:
        f5.update(new_state);
        break;
    case VK_F6:
        f6.update(new_state);
        break;
    case VK_F7:
        f7.update(new_state);
        break;
    case VK_F8:
        f8.update(new_state);
        break;
    case VK_F9:
        f9.update(new_state);
        break;
    case VK_F10:
        f10.update(new_state);
        break;
    case VK_F11:
        f11.update(new_state);
        break;
    case VK_F12:
        f12.update(new_state);
        break;
    }
}

void kl::keyboard::process() const
{
    q.process();
    w.process();
    e.process();
    r.process();
    t.process();
    z.process();
    u.process();
    i.process();
    o.process();
    p.process();
    a.process();
    s.process();
    d.process();
    f.process();
    g.process();
    h.process();
    j.process();
    k.process();
    l.process();
    y.process();
    x.process();
    c.process();
    v.process();
    b.process();
    n.process();
    m.process();

    num0.process();
    num1.process();
    num2.process();
    num3.process();
    num4.process();
    num5.process();
    num6.process();
    num7.process();
    num8.process();
    num9.process();

    period.process();
    comma.process();
    plus.process();
    minus.process();

    esc.process();
    tab.process();
    caps.process();
    shift.process();
    ctrl.process();
    alt.process();
    space.process();
    enter.process();
    insert.process();
    delet.process();

    up.process();
    left.process();
    down.process();
    right.process();

    f1.process();
    f2.process();
    f3.process();
    f4.process();
    f5.process();
    f6.process();
    f7.process();
    f8.process();
    f9.process();
    f10.process();
    f11.process();
    f12.process();
}
