#ifndef SRENDER_RGBCOLOR
#define SRENDER_RGBCOLOR

namespace SRender 
{
	class RGBColor
	{
	public:
		unsigned char red = 0;
		unsigned char green = 0;
		unsigned char blue = 0;
	};

	class RGBAColor
	{
	public:
		unsigned char blue = 0;
		unsigned char green = 0;
		unsigned char red = 0;
		unsigned char alpha = 0;

		RGBAColor() = default;
		RGBAColor(unsigned char red,
			unsigned char green,
			unsigned char blue,
			unsigned char alpha) :
			red(red), green(green), blue(blue), alpha(alpha)
		{

		}

		RGBAColor(const RGBAColor& color)
		{
			red = color.red;
			green = color.green;
			blue = color.blue;
			alpha = color.alpha;
		}

		RGBAColor operator + (const RGBAColor& other)
		{
			RGBAColor res;
			res.red = (this->red * (255 - other.alpha) + other.red * other.alpha) / 255;
			res.green = (this->green * (255 - other.alpha) + other.green * other.alpha) / 255;
			res.blue = (this->blue * (255 - other.alpha) + other.blue * other.alpha) / 255;
			/*res.red = (Min<unsigned char>((red * alpha / 255) + (other.red * other.alpha * (255 - alpha) / 65025), 255));
			res.green = (Min<unsigned char>((green * alpha / 255) + (other.green * other.alpha * (255 - alpha) / 65025), 255));
			res.blue = (Min<unsigned char>((blue * alpha / 255) + (other.blue * other.alpha * (255 - alpha) / 65025), 255));*/
			res.alpha = 255;
			return res;
		}

		RGBAColor& operator = (const RGBAColor& other)
		{
			red = other.red;
			green = other.green;
			blue = other.blue;
			alpha = other.alpha;
			return *this;
		}

		bool operator == (const RGBAColor& other)
		{
			if (red == other.red &&
				blue == other.blue &&
				green == other.green &&
				alpha == other.alpha)
			{
				return true;
			}
			return false;
		}

		bool operator != (const RGBAColor& other)
		{
			return !this->operator==(other);
		}

		static RGBAColor LinarAdjust(RGBAColor& color1, RGBAColor& color2, double k)
		{
			RGBAColor res;
			res.red = color1.red * k + color2.red * (1 - k);
			res.green = color1.green * k + color2.green * (1 - k);
			res.blue = color1.blue * k + color2.blue * (1 - k);
			res.alpha = color1.alpha * k + color2.alpha * (1 - k);
			return res;
		}

		static RGBAColor LinarAdjust(RGBAColor& color1, RGBAColor& color2, 
			RGBAColor& color3, RGBAColor& color4, double kx, double ky)
		{
			RGBAColor colorTemp1 = LinarAdjust(color1, color2, kx);
			RGBAColor colorTemp2 = LinarAdjust(color3, color4, kx);
			return LinarAdjust(colorTemp1, colorTemp2, ky);
		}
	};
}
#endif // !SRENDER_RGBCOLOR



