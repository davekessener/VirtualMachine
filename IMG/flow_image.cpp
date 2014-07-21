template<typename T>
flow_image<T>::flow_image(const std::string& _fn, const basic_image<T>& img) : 
	basic_image<T>(img.width, 0), 
	fn(_fn), 
	file(fn, std::ios::out | std::ios::binary)
{
	this->height = img.height;

	assert(file.is_open());

	memset(&bmh, 0, sizeof(bitmap_header_t));
	memset(&dib, 0, sizeof(DIB_t));

	file.write((const char *) &bmh, sizeof(bitmap_header_t));
	file.write((const char *) &dib, sizeof(DIB_t));

	img.write(file);
}

template<typename T>
flow_image<T>::flow_image(const std::string& _fn, int _w) : basic_image<T>(_w, 0), fn(_fn), file(fn, std::ios::out | std::ios::binary)
{
	assert(file.is_open());

	memset(&bmh, 0, sizeof(bitmap_header_t));
	memset(&dib, 0, sizeof(DIB_t));

	file.write((const char *) &bmh, sizeof(bitmap_header_t));
	file.write((const char *) &dib, sizeof(DIB_t));
}

template<typename T>
flow_image<T>::~flow_image(void)
{
	if(file.is_open())
	{
		close();
	}
}

template<typename T>
void flow_image<T>::draw(const basic_image<T>& img)
{
	if(this->width != img.width)
	{
#ifdef DEBUG
		std::cerr << "ERR: Trying to write to flow_image of width " << this->width
			<< " with an image of width " << img.width << "!" << std::endl;
#endif
		return;
	}

	this->height += img.height;
	img.write(file);
}

template<typename T>
void flow_image<T>::flush(void)
{
	file.seekp(0, std::ios::beg);

	this->prepareDIB(&dib);
	this->prepareBMH(&bmh, &dib);

	file.write((const char *) &bmh, sizeof(bitmap_header_t));
	file.write((const char *) &dib, sizeof(DIB_t));

	file.seekp(0, std::ios::end);
}

template<typename T>
void flow_image<T>::close(void)
{
	flush();
	file.close();
}

