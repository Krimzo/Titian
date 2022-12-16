package utility

class Pair<T> {
    var first: T?
    var second: T?

    constructor() {
        first = null
        second = null
    }

    constructor(first: T, second: T) {
        this.first = first
        this.second = second
    }
}