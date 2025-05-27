mod Math {
    add(Int(a), Int(b)): Int =
        a + b;

    sub(Int(a), Int(b)): Int =
        a - b;

    mul(Int(a), Int(b)): Int =
        a * b;

    safeDiv(Int(a), Int(b)): Int =
        if b <= Int(0) then String("Divide by zero") else a / b;
};