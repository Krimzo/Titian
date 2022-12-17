package math

import java.io.Serializable

class Plane : Serializable {
    var normal: Float3 = Float3()
    var point: Float3 = Float3()

    constructor()

    constructor(normal: Float3, point: Float3) {
        this.normal = normal
        this.point = point
    }

    constructor(plane: Plane) {
        normal = Float3(plane.normal)
        point = Float3(plane.point)
    }
}