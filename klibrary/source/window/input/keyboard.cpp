#include "klibrary.h"


void kl::Keyboard::_reload()
{
    for (auto& key : keys) {
		key._reload();
    }
}

void kl::Keyboard::_update(const WPARAM type, const bool new_state)
{
    char type_char = (char) MapVirtualKeyA(UINT(type), MAPVK_VK_TO_CHAR);
    if (type_char != 0) {
        _update_char(type_char, new_state);
    }
    else {
        _update_virtual(type, new_state);
    }
}

void kl::Keyboard::_update_char(const char type, const bool new_state)
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
    case '0': zero._update(new_state); break;
    case '1': one._update(new_state); break;
    case '2': two._update(new_state); break;
    case '3': three._update(new_state); break;
    case '4': four._update(new_state); break;
    case '5': five._update(new_state); break;
    case '6': six._update(new_state); break;
    case '7': seven._update(new_state); break;
    case '8': eight._update(new_state); break;
    case '9': nine._update(new_state); break;
	case '.': period._update(new_state); break;
	case ',': comma._update(new_state); break;
	case '+': plus._update(new_state); break;
	case '-': minus._update(new_state); break;
	case '*': multiply._update(new_state); break;
	case '/': divide._update(new_state); break;
	case '=': equals._update(new_state); break;
	case '[': lsqrbrckt._update(new_state); break;
	case ']': rsqrbrckt._update(new_state); break;
	case ';': semicolon._update(new_state); break;
	case '\'': apostrophe._update(new_state); break;
	case '\\': backslash._update(new_state); break;
	case '`': backtick._update(new_state); break;
    case '\t': tab._update(new_state); break;
    case ' ': space._update(new_state); break;
    case '\r': enter._update(new_state); break;
    case '\b': backspace._update(new_state); break;
    case '\x1b': esc._update(new_state); break;
    }
}

void kl::Keyboard::_update_virtual(const WPARAM type, const bool new_state)
{
    switch (type)
    {
    case VK_CAPITAL: caps._update(new_state); break;
    case VK_SHIFT: shift._update(new_state); break;
    case VK_CONTROL: ctrl._update(new_state); break;
    case VK_MENU: alt._update(new_state); break;
    case VK_INSERT: insert._update(new_state); break;
    case VK_DELETE: delet._update(new_state); break;
    case VK_HOME: home._update(new_state); break;
    case VK_END: end._update(new_state); break;
    case VK_PRIOR: pageup._update(new_state); break;
    case VK_NEXT: pagedown._update(new_state); break;
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
