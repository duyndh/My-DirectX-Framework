#pragma once

typedef struct Bound
{
	float top, bottom, left, right;

	Bound()
	{
		top = left = 0;
		right = bottom = 1;
	}
	Bound(float top, float left, float bottom, float right)
	{
		this->top = top;
		this->left = left;
		this->bottom = bottom;
		this->right = right;
	}
	Bound(Vector2 topLeft, Vector2 size)
	{
		if (size.x >= 0)
		{
			left = topLeft.x;
			right = left + size.x;
		}else
		{
			right = topLeft.x;
			left = right + size.x;
		}

		if(size.y>=0)
		{
			top = topLeft.y;
			bottom = top + size.y;
		}else
		{
			bottom = topLeft.y;
			top = bottom + size.y;
		}
	}
	~Bound() = default;

	Vector2 TopLeft() const { return Vector2(top, left); }
	Vector2 Size() const { return Vector2(right - left, bottom - top); }

	void GetBound(float &t, float &l, float &r, float &b) const
	{
		t = top; l = left; r = right; b = bottom;
	}

	RECT ToRECT() const
	{
		const RECT rect = { left, top, right, bottom };
		return rect;
	}

	bool intersect(Bound other) const
	{
		const float m_left = other.left - right;
		const float m_top = other.bottom - top;
		const float m_right = other.right - left;
		const float m_bottom = other.top - bottom;

		return !(m_left > 0 || m_right < 0 || m_top < 0 || m_bottom > 0);
	}

	Bound OverLapBound(Bound other)
	{
		const float _top = top > other.top ? top : other.top;
		const float _bottom = bottom < other.bottom ? bottom : other.bottom;
		const float _left = left > other.left ? left : other.left;
		const float _right = right < other.right ? right : other.right;

		if (_top > _bottom || _left > _right) return Bound(0, 0, 0, 0);

		return Bound(_top, _left, _bottom, _right);
	}

	bool isInside(Vector2 point) const
	{
		return !(point.x < left || point.x > right || point.y < top || point.y > bottom);
	}

	Bound& operator= (const Bound &_bound) = default;
	bool operator== (const Bound &_bound) const
	{
		return !(left != _bound.left || right != _bound.right || top != _bound.top || bottom != _bound.bottom);
	}
	bool operator!= (const Bound &_bound) const
	{
		return (left != _bound.left || right != _bound.right || top != _bound.top || bottom != _bound.bottom);
	}
};
