
template<typename Id, typename Resource> ResourceLoader<typename Id, typename Resource>::ResourceLoader(){}
template<typename Id, typename Resource> ResourceLoader<typename Id, typename Resource>::~ResourceLoader(){}



template<typename Id, typename Resource> void ResourceLoader<typename Id, typename Resource>::LoadResource(Id id, const std::string filename)	
	{
	std::unique_ptr<Resource> resource = std::make_unique<Resource>();

	if (!resource->loadFromFile(filename))
		{
		throw std::runtime_error("Failed to load image.");
		}
	resources.insert(std::make_pair(id, std::move(resource)));
	}

template<typename Id, typename Resource> Resource& ResourceLoader<typename Id, typename Resource>::Get(Id id)
	{
	auto found = resources.find(id);
	if (found == resources.end())
		{
		//error handling
		}
	else return *found->second; 

	}