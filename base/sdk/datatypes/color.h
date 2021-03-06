#pragma once
// used: std::array
#include <array>

struct ColorRGBExp32
{
	std::byte r, g, b;
	signed char exponent;
};

class Color
{
public:
	Color() = default;

	Color(std::uint8_t r, std::uint8_t g, std::uint8_t b)
	{
		Set(r, g, b, 255);
	}

	Color(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a)
	{
		Set(r, g, b, a);
	}

	void Set(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a)
	{
		arrColor.at(0) = r;
		arrColor.at(1) = g;
		arrColor.at(2) = b;
		arrColor.at(3) = a;
	}

	void Set(float r, float g, float b, float a)
	{
		arrColor.at(0) = static_cast<std::uint8_t>(r * 255.f);
		arrColor.at(1) = static_cast<std::uint8_t>(g * 255.f);
		arrColor.at(2) = static_cast<std::uint8_t>(b * 255.f);
		arrColor.at(3) = static_cast<std::uint8_t>(a * 255.f);
	}

	void Get(std::uint8_t& r, std::uint8_t& g, std::uint8_t& b, std::uint8_t& a) const
	{
		r = arrColor.at(0);
		g = arrColor.at(1);
		b = arrColor.at(2);
		a = arrColor.at(3);
	}

	/* convert color to directx rgba */
	ImU32 GetU32()
	{
		return ImGui::GetColorU32(ImVec4(this->rBase(), this->gBase(), this->bBase(), this->aBase()));
	}

	bool operator==(const Color& colSecond) const
	{
		return *const_cast<Color*>(this) == *const_cast<Color*>(&colSecond);
	}

	bool operator!=(const Color& colSecond) const
	{
		return !(operator==(colSecond));
	}

	Color& operator=(const Color& colFrom)
	{
		Set(colFrom.r(), colFrom.g(), colFrom.b(), colFrom.a());
		return *this;
	}

	inline std::uint8_t r() const { return arrColor.at(0); }
	inline std::uint8_t g() const { return arrColor.at(1); }
	inline std::uint8_t b() const { return arrColor.at(2); }
	inline std::uint8_t a() const { return arrColor.at(3); }

	inline float rBase() const { return arrColor.at(0) / 255.f; }
	inline float gBase() const { return arrColor.at(1) / 255.f; }
	inline float bBase() const { return arrColor.at(2) / 255.f; }
	inline float aBase() const { return arrColor.at(3) / 255.f; }

	std::array<float, 3U> Base()
	{
		std::array<float, 3U> arrBaseColor = { };
		arrBaseColor.at(0) = arrColor.at(0) / 255.f;
		arrBaseColor.at(1) = arrColor.at(1) / 255.f;
		arrBaseColor.at(2) = arrColor.at(2) / 255.f;
		return arrBaseColor;
	}

	static Color FromBase3(float rgb[3])
	{
		return Color(static_cast<std::uint8_t>(rgb[0] * 255.f), static_cast<std::uint8_t>(rgb[1] * 255.f), static_cast<std::uint8_t>(rgb[2] * 255.f));
	}

	std::array<float, 4U> BaseAlpha()
	{
		std::array<float, 4U> arrBaseColor = { };
		arrBaseColor.at(0) = arrColor.at(0) / 255.f;
		arrBaseColor.at(1) = arrColor.at(1) / 255.f;
		arrBaseColor.at(2) = arrColor.at(2) / 255.f;
		arrBaseColor.at(3) = arrColor.at(3) / 255.f;
		return arrBaseColor;
	}

	static Color FromBase4(float rgba[4])
	{
		return Color(static_cast<std::uint8_t>(rgba[0] * 255.f), static_cast<std::uint8_t>(rgba[1] * 255.f), static_cast<std::uint8_t>(rgba[2] * 255.f), static_cast<std::uint8_t>(rgba[3] * 255.f));
	}

	float Hue() const
	{
		if (arrColor.at(0) == arrColor.at(1) && arrColor.at(1) == arrColor.at(2))
			return 0.f;

		const float r = arrColor[0] / 255.f;
		const float g = arrColor[1] / 255.f;
		const float b = arrColor[2] / 255.f;

		const float flMax = std::max(r, std::max(g, b)), flMin = std::min(r, std::min(g, b));

		if (flMax == flMin)
			return 0.f;

		const float flDelta = flMax - flMin;
		float flHue = 0.f;

		if (flMax == r)
			flHue = (g - b) / flDelta;
		else if (flMax == g)
			flHue = 2.f + (b - r) / flDelta;
		else if (flMax == b)
			flHue = 4.f + (r - g) / flDelta;

		flHue *= 60.f;

		if (flHue < 0.f)
			flHue += 360.f;

		return flHue / 360.f;
	}

	float Saturation() const
	{
		const float r = arrColor.at(0) / 255.f;
		const float g = arrColor.at(1) / 255.f;
		const float b = arrColor.at(2) / 255.f;

		const float flMax = std::max(r, std::max(g, b)), flMin = std::min(r, std::min(g, b));
		const float flDelta = flMax - flMin;

		if (flMax == 0.f)
			return flDelta;

		return flDelta / flMax;
	}

	float Brightness() const
	{
		const float r = arrColor.at(0) / 255.f;
		const float g = arrColor.at(1) / 255.f;
		const float b = arrColor.at(2) / 255.f;

		return std::max(r, std::max(g, b));
	}

	static Color FromHSB(float flHue, float flSaturation, float flBrightness)
	{
		float r = 0.0f, g = 0.0f, b = 0.0f;
		const float h = std::fmodf(flHue, 1.0f) / (60.0f / 360.0f);
		const int i = static_cast<int>(h);
		const float f = h - static_cast<float>(i);
		const float p = flBrightness * (1.0f - flSaturation);
		const float q = flBrightness * (1.0f - flSaturation * f);
		const float t = flBrightness * (1.0f - flSaturation * (1.0f - f));

		switch (i)
		{
		case 0:
			r = flBrightness, g = t, b = p;
			break;
		case 1:
			r = q, g = flBrightness, b = p;
			break;
		case 2:
			r = p, g = flBrightness, b = t;
			break;
		case 3:
			r = p, g = q, b = flBrightness;
			break;
		case 4:
			r = t, g = p, b = flBrightness;
			break;
		case 5:
		default:
			r = flBrightness, g = p, b = q;
			break;
		}

		return Color(static_cast<std::uint8_t>(r * 255.f), static_cast<std::uint8_t>(g * 255.f), static_cast<std::uint8_t>(b * 255.f));
	}

	std::array<std::uint8_t, 4U> arrColor;
};
