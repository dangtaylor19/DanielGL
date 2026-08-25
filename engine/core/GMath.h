#include <Vector3.h>

class GMath {
public:
	static float Clamp(float a, float min, float max) {
		if (a < min) {
			a = min;
		}
		else if (a > max) {
			a = max;
		}

		return a;
	}

	static float Lerp(float a, float b, float t) {
		return a + (b - a) * Clamp(t, 0.0f, 1.0f);
	}

	static Vector3 Vec3Lerp(Vector3 a, Vector3 b, float t) {
		return Vector3(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t);
	}

	static Vector3 MoveTo(Vector3 a, Vector3 b, float t) {
		Vector3 direction = b - a;
		direction.normalize();
		return a + direction * t;
	}
};
