#include "klibrary.h"


void kl::Keyboard::_reload()
{
    for (auto& key : keys) {
		key._reload();
    }
}

void kl::Keyboard::_update(const uint64_t type, const bool new_state)
{
    switch (type)
    {
    case 'Q': q._update(new_state); break;
    case 'W': w._update(new_state); break;
    case 'E': e._update(new_state); break;
    case 'R': r._update(new_state); break;
    case 'T': t._update(new_state); break;
    case 'Z': z._update(new_state); break;
    case 'U': u._update(new_state); break;
    case 'I': i._update(new_state); break;
    case 'O': o._update(new_state); break;
    case 'P': p._update(new_state); break;
    case 'A': a._update(new_state); break;
    case 'S': s._update(new_state); break;
    case 'D': d._update(new_state); break;
    case 'F': f._update(new_state); break;
    case 'G': g._update(new_state); break;
    case 'H': h._update(new_state); break;
    case 'J': j._update(new_state); break;
    case 'K': k._update(new_state); break;
    case 'L': l._update(new_state); break;
    case 'Y': y._update(new_state); break;
    case 'X': x._update(new_state); break;
    case 'C': c._update(new_state); break;
    case 'V': v._update(new_state); break;
    case 'B': b._update(new_state); break;
    case 'N': n._update(new_state); break;
    case 'M': m._update(new_state); break;

    case '0': num0._update(new_state); break;
    case '1': num1._update(new_state); break;
    case '2': num2._update(new_state); break;
    case '3': num3._update(new_state); break;
    case '4': num4._update(new_state); break;
    case '5': num5._update(new_state); break;
    case '6': num6._update(new_state); break;
    case '7': num7._update(new_state); break;
    case '8': num8._update(new_state); break;
    case '9': num9._update(new_state); break;

    case VK_OEM_PERIOD: period._update(new_state); break;
    case VK_OEM_COMMA: comma._update(new_state); break;
    case VK_OEM_PLUS: plus._update(new_state); break;
    case VK_OEM_MINUS: minus._update(new_state); break;

    case VK_ESCAPE: esc._update(new_state); break;
    case VK_TAB: tab._update(new_state); break;
    case VK_CAPITAL: caps._update(new_state); break;
    case VK_SHIFT: shift._update(new_state); break;
    case VK_CONTROL: ctrl._update(new_state); break;
    case VK_MENU: alt._update(new_state); break;
    case VK_SPACE: space._update(new_state); break;
    case VK_RETURN: enter._update(new_state); break;
    case VK_INSERT: insert._update(new_state); break;
    case VK_DELETE: delet._update(new_state); break;

    case VK_UP: up._update(new_state); break;
    case VK_LEFT: left._update(new_state); break;
    case VK_DOWN: down._update(new_state); break;
    case VK_RIGHT: right._update(new_state); break;

    case VK_F1: f1._update(new_state); break;
    case VK_F2: f2._update(new_state); break;
    case VK_F3: f3._update(new_state); break;
    case VK_F4: f4._update(new_state); break;
    case VK_F5: f5._update(new_state); break;
    case VK_F6: f6._update(new_state); break;
    case VK_F7: f7._update(new_state); break;
    case VK_F8: f8._update(new_state); break;
    case VK_F9: f9._update(new_state); break;
    case VK_F10: f10._update(new_state); break;
    case VK_F11: f11._update(new_state); break;
    case VK_F12: f12._update(new_state); break;
    }
}
