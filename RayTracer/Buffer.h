#ifndef __BUFFER
#define __BUFFER

#include "GenVector.h"

class Buffer
{
public:
    Buffer()
    {
        this->w = 0;
        this->h = 0;
        this->data = NULL;
        // printf("made an empty buffer\n");
    }

    Buffer(unsigned int w, unsigned int h)
    {
        this->w = w;
        this->h = h;
        // printf("made a not empty buffer\n");

        alloc();
    }

    Buffer(Buffer const &buffer)
    {
        // printf("made a copied buffer\n");
        copy(buffer);
    }

    Buffer &operator=(Buffer const &buffer)
    {
        // printf("dealloc from copy at 0x%x\n", this->data);
        if (this == &buffer)
            return *this;
        
        dealloc();
        copy(buffer);

        return *this;
    }

    ~Buffer()
    {
        // printf("destructor called for the buffer data at 0x%x\n", this->data);
        dealloc();
    }

    Color at(unsigned int x, unsigned int y) const
    {
        return data[x + y * this->w];
    }

    Color &at(unsigned int x, unsigned int y)
    {
        return data[x + y * this->w];
    }

    size_t getWidth() const
    {
        return this->w;
    }

    size_t getHeight() const
    {
        return this->h;
    }

private:
    unsigned int w;
    unsigned int h;
    Color *data;

    void alloc()
    {
        size_t size = this->w * this->h;
        if (size == 0)
            return;

        data = (Color *) malloc(this->w * this->h * sizeof(Color) );
    }

    void dealloc()
    {
        
        if (data != NULL)
        {
            free(data);
        }
        data = NULL;
    }

    void copy(Buffer const &buffer)
    {
        this->w = buffer.w;
        this->h = buffer.h;

        alloc();
        for (size_t i = 0; i < this->w * this->h; i++)
            this->data[i] = buffer.data[i];
    }
};

#endif
