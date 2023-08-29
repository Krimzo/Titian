package sdk.render.components.shape

import sdk.math.vector.Float3

class SphereShape : Shape(ShapeType.SPHERE) {
    var radius: Float = 1f

    override fun getGeometry(): Float3 {
        return Float3(radius, -1f, -1f)
    }
}
