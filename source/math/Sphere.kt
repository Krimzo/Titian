package math

import java.io.Serializable

class Sphere : Serializable {
    var center: Float3
    var radius: Float

    constructor(center: Float3, radius: Float) {
        this.center = center
        this.radius = radius
    }

    constructor(sphere: Sphere) {
        center = Float3(sphere.center)
        radius = sphere.radius
    }
}