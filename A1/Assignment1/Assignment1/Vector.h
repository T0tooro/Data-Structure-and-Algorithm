#pragma once
#include <stdexcept>

/**
 * @class Vector
 * @brief Custom minimalist templated linear container encapsulating a heap array.
 * Contains zero inline function body definitions within the class layout structure.
 *
 * @tparam T Type of the element stored within the container array.
 */
template <class T>
class Vector
{
public:
    /**
     * @brief Default constructor. Initializes an empty Vector with no heap allocation.
     */
    Vector();

    /**
     * @brief Parameterized capacity constructor. Allocates initial memory tracking block.
     * @param n Initial count allocations to preserve before resizing.
     */
    explicit Vector(int n);

    /**
     * @brief Copy constructor. Orchestrates complete deep memory duplication paths.
     * @param other Core reference instances targeted for duplication.
     */
    Vector(const Vector<T> & other);

    /**
     * @brief Copy-assignment operator. Manages resource cleanup and assignment safely.
     * @param other Core reference instances targeted for assignment.
     * @return Reference to the updated instance.
     */
    Vector<T> & operator=(const Vector<T> & other);

    /**
     * @brief Destructor. Safely cleans up allocated heap memory resources.
     */
    ~Vector();

    /**
     * @brief Retrieves the count of elements currently loaded.
     * @return Element count as a signed integer primitive.
     */
    int Size() const;

    /**
     * @brief Non-constant subscript element access operator.
     * Allows reference modifications to values inside range-validated boundaries.
     * @param index Explicit target position offset sequence requested.
     * @return Mutable reference pointing directly to the target elements.
     * @throws std::out_of_range If the requested index is out of bounds.
     */
    T & operator[](int index);

    /**
     * @brief Constant subscript element access operator.
     * Protects internal data structure integrity during inspection routines.
     * @param index Explicit target position offset sequence requested.
     * @return Immutable reference pointing directly to the target elements.
     * @throws std::out_of_range If the requested index is out of bounds.
     */
    const T & operator[](int index) const;

    /**
     * @brief Direct positioning insertion routine. Automatically handles capacity expansion.
     * @param index Targeted array sequence index for placement.
     * @param data Reference parameter tracking content intended for structural updates.
     * @return True if insertion succeeds, false otherwise.
     */
    bool Insert(int index, const T & data);

private:
    /**
     * @brief Internal resizing mechanic. Doubles heap array capacity upon saturation.
     */
    void Resize();

    T * m_vector;
    int m_capacity;
    int m_size;
};

// --- IMPLEMENTATIONS OUTSIDE THE CLASS BODY (NON-INLINE) ---

template <class T>
Vector<T>::Vector() : m_vector{nullptr}, m_capacity{0}, m_size{0}
{
}

template <class T>
Vector<T>::Vector(int n)
{
    if (n < 1)
        n = 1; // safeguard from trolls
    m_capacity = n;
    m_size = 0;
    m_vector = new T[m_capacity];
}

template <class T>
Vector<T>::Vector(const Vector<T> & other)
    : m_vector{new T[other.m_capacity]}, m_capacity{other.m_capacity}, m_size{other.m_size}
{
    for (int i{0}; i < m_size; ++i)
    {
        m_vector[i] = other.m_vector[i];
    }
}

template <class T>
Vector<T>& Vector<T>::operator=(const Vector<T> & other)
{
    if (this != &other)
    {
        delete [] m_vector;
        m_capacity = other.m_capacity;
        m_size = other.m_size;
        m_vector = new T[m_capacity];
        for (int i{0}; i < m_size; ++i)
        {
            m_vector[i] = other.m_vector[i];
        }
    }
    return *this;
}

template <class T>
Vector<T>::~Vector()
{
    delete [] m_vector;
    m_vector = nullptr;
}

template <class T>
int Vector<T>::Size() const
{
    return m_size;
}

template <class T>
T & Vector<T>::operator[](int index)
{
    if (index < 0 || index >= m_size)
    {
        throw std::out_of_range("Vector index out of bounds.");
    }
    return m_vector[index];
}

template <class T>
const T & Vector<T>::operator[](int index) const
{
    if (index < 0 || index >= m_size)
    {
        throw std::out_of_range("Vector constant index out of bounds.");
    }
    return m_vector[index];
}

template <class T>
void Vector<T>::Resize()
{
    int newCapacity{ (m_capacity == 0) ? 1 : m_capacity * 2 };
    T * temp{ new T[newCapacity] };
    for (int i{0}; i < m_size; ++i)
    {
        temp[i] = m_vector[i];
    }
    delete [] m_vector;
    m_vector = temp;
    m_capacity = newCapacity;
}

template <class T>
bool Vector<T>::Insert(int index, const T & data)
{
    if (index < 0 || index > m_size)
    {
        return false;
    }
    if (m_size >= m_capacity)
    {
        Resize();
    }
    for (int i{m_size}; i > index; --i)
    {
        m_vector[i] = m_vector[i - 1];
    }
    m_vector[index] = data;
    ++m_size;
    return true;
}
