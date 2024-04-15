#ifndef BASE_CONTAINER_HPP
#define BASE_CONTAINER_HPP


#include <vector>
#include <memory>

template<typename BaseType>
class BaseContainer {
private:
	using Container = std::vector<std::shared_ptr<BaseType>>;

public:
	BaseContainer() noexcept {}
	



};


#endif