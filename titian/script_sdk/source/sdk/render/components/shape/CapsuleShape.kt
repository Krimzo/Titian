package sdk.render.components.shape

import sdk.math.vector.Float3

class CapsuleShape : Shape(ShapeType.CAPSULE) {
    var radius: Float = 1f
    var height: Float = 1f

    override fun getGeometry(): Float3 {
        return Float3(radius, height, -1f)
    }
}
