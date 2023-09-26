package sdk.render.components.shape

import sdk.math.vector.Float3

abstract class Shape(val type: Int) {
    abstract fun getGeometry(): Float3
}
