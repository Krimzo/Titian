package sdk.render.scene

import sdk.math.vector.Float3

class Entity(val uniqueIndex: Long) {
    var renderScale: Float3 = Float3()

    var mesh: String = ""
    var material: String = ""
}
