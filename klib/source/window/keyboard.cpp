#include "window/keyboard.h"

#include "window/system_handler.h"


kl::keyboard::keyboard() :
    q('Q'),
    w('W'),
    e('E'),
    r('R'),
    t('T'),
    z('Z'),
    u('U'),
    i('I'),
    o('O'),
    p('P'),
    a('A'),
    s('S'),
    d('D'),
    f('F'),
    g('G'),
    h('H'),
    j('J'),
    k('K'),
    l('L'),
    y('Y'),
    x('X'),
    c('C'),
    v('V'),
    b('B'),
    n('N'),
    m('M'),

    num0('0'),
    num1('1'),
    num2('2'),
    num3('3'),
    num4('4'),
    num5('5'),
    num6('6'),
    num7('7'),
    num8('8'),
    num9('9'),

    period(VK_OEM_PERIOD),
    comma(VK_OEM_COMMA),
    plus(VK_OEM_PLUS),
    minus(VK_OEM_MINUS),

    esc(VK_ESCAPE),
    tab(VK_TAB),
    caps(VK_CAPITAL),
    shift(VK_SHIFT),
    ctrl(VK_CONTROL),
    alt(VK_MENU),
    space(VK_SPACE),
    enter(VK_RETURN),
    insert(VK_INSERT),
    delet(VK_DELETE),
    up(VK_UP),
    left(VK_LEFT),
    down(VK_DOWN),
    right(VK_RIGHT),

    f1(VK_F1),
    f2(VK_F2),
    f3(VK_F3),
    f4(VK_F4),
    f5(VK_F5),
    f6(VK_F6),
    f7(VK_F7),
    f8(VK_F8),
    f9(VK_F9),
    f10(VK_F10),
    f11(VK_F11),
    f12(VK_F12)
{}

kl::keyboard::~keyboard()
{}

void kl::keyboard::update_value(const WPARAM type, const bool state)
{
    for (auto& key : buffer) {
        key.update_value(type, state);
    }
}

void kl::keyboard::process() const
{
    for (auto& key : buffer) {
        key.process();
    }
}
