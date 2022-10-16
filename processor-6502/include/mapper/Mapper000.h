#include "IMapper.h"

class Mapper000 : public IMapper {
 public:
  virtual ~Mapper000() = default;

 public:
  virtual MappingResult MapRead(uint16_t address) override;
  virtual MappingResult MapWrite(uint16_t address) override;
};
