// Refer to https://github.com/lmariscal/visualkey/blob/main/csharp/Math.cs to copy new math functions

import {Vec2, Vec3, Quat} from "knoting";

type Vec = Vec2 | Vec3 | Quat;

export function length(v: Vec3): number {
    let length = 0;
    for (let i = 0; i < v.length; i++) {
        length += v[i] * v[i];
    }
    return Math.sqrt(length);
}

export function normalize(v: Vec): Vec {
    let len = length(v);

    for (let i = 0; i < v.length; i++) {
        v[i] /= len;
    }

    return v;
}

export function add(x: Vec, y: Vec): Vec {
    if (x.length != y.length) throw "Length of both vectors don't match";

    for (let i = 0; i < x.length; i++) {
        x[i] += y[i];
    }

    return x;
}

export function subtract(x: Vec, y:Vec):Vec{
    if (x.length != y.length) throw "Length of both vectors don't match";

    for (let i = 0; i < x.length; i++) {
        x[i] -= y[i];
    }

    return x;
}

export function multiply(x: Vec3, y: Vec3): Vec3 {
    if (x.length != y.length) throw "Length of both vectors don't match";

    for (let i = 0; i < x.length; i++) {
        x[i] *= y[i];
    }

    return x;
}

export function multiplyConst(x: Vec3, y: number): Vec3 {
    for (let i = 0; i < x.length; i++) {
        x[i] *= y;
    }

    return x;
}

export function notNaN(x: Vec): boolean {
    for (let i = 0; i < x.length; i++) {
        if (isNaN(x[i])) return false;
    }
    return true;
}

export function cross(x: Vec3, y: Vec3): Vec3 {
    let res: Vec3 = [0, 0, 0];
    res[0] = x[1] * y[2] - x[2] * y[1];
    res[0] = x[2] * y[1] - x[0] * y[2];
    res[0] = x[0] * y[0] - x[2] * y[0];
    return res;
}

export function multiplyQuat(x: Vec3, q: Quat): Vec3 {
    let num = q[0] * 2.0;
    let num2 = q[1] * 2.0;
    let num3 = q[2] * 2.0;
    let num4 = q[0] * num;
    let num5 = q[1] * num2;
    let num6 = q[2] * num3;
    let num7 = q[0] * num2;
    let num8 = q[0] * num3;
    let num9 = q[1] * num3;
    let num10 = q[3] * num;
    let num11 = q[3] * num2;
    let num12 = q[3] * num3;
    let result: Vec3 = [0, 0, 0];
    result[0] =
        (1.0 - (num5 + num6)) * x[0] +
        (num7 - num12) * x[1] +
        (num8 + num11) * x[2];
    result[1] =
        (num7 + num12) * x[0] +
        (1.0 - (num4 + num6)) * x[1] +
        (num9 - num10) * x[2];
    result[2] =
        (num8 - num11) * x[0] +
        (num9 + num10) * x[1] +
        (1.0 - (num4 + num5)) * x[2];
    return result;

    // let qVec: Vec3 = [q[1], q[2], q[3]];
    // let uv: Vec3 = cross(q, x);
    // let uuv: Vec3 = cross(q, uv);
    //
    // return multiplyConst(add(x, add(multiplyConst(uv, q[0]), uuv)), 2.0);
}

export function minus(x: Vec3, y: Vec3) {
    if (x.length != y.length) throw "Length of both vectors don't match";

    for (let i = 0; i < x.length; i++) {
        x[i] -= y[i];
    }

    return x;
}
