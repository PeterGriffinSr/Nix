mod Math {
    sqaure(Int(x)) : Int = x * x;

    safeDiv(Int(a), Int(b)) : Int =
        if Int(b) <= Int(0) then String("Divide by zero") else Int(a / b);
};