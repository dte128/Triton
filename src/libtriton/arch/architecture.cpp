//! \file
/*
**  Copyright (C) - Triton
**
**  This program is under the terms of the BSD License.
*/

#include <api.hpp>
#include <architecture.hpp>
#include <exceptions.hpp>
#include <x8664Cpu.hpp>
#include <x86Cpu.hpp>



namespace triton {
  namespace arch {

    Architecture::Architecture() {
      this->arch = triton::arch::ARCH_INVALID;
      this->cpu  = nullptr;
    }


    Architecture::~Architecture() {
      delete this->cpu;
    }


    triton::uint32 Architecture::getArchitecture(void) const {
      return this->arch;
    }


    triton::arch::CpuInterface* Architecture::getCpu(void) {
      if (!this->cpu)
        throw triton::exceptions::Architecture("Architecture::getCpu(): CPU undefined.");
      return this->cpu;
    }


    void Architecture::setArchitecture(triton::uint32 arch) {
      /* Check if the architecture is valid */
      if (!(arch > triton::arch::ARCH_INVALID && arch < triton::arch::ARCH_LAST_ITEM))
        throw triton::exceptions::Architecture("Architecture::setArchitecture(): Invalid architecture.");

      /* Setup global variables */
      this->arch = arch;

      /* Allocate and init the good arch */
      switch (this->arch) {
        case triton::arch::ARCH_X86_64:
          /* remove previous CPU instance (when setArchitecture() has been called twice) */
          delete this->cpu;
          /* init the new instance */
          this->cpu = new triton::arch::x86::x8664Cpu();
          if (!this->cpu)
            throw triton::exceptions::Architecture("Architecture::setArchitecture(): Not enough memory.");
          this->cpu->init();
          break;

        case triton::arch::ARCH_X86:
          /* remove previous CPU instance (when setArchitecture() has been called twice) */
          delete this->cpu;
          /* init the new instance */
          this->cpu = new triton::arch::x86::x86Cpu();
          if (!this->cpu)
            throw triton::exceptions::Architecture("Architecture::setArchitecture(): Not enough memory.");
          this->cpu->init();
          break;
      }
    }


    void Architecture::clearArchitecture(void) {
      if (!this->cpu)
        throw triton::exceptions::Architecture("Architecture::clearArchitecture(): You must define an architecture.");
      this->cpu->clear();
    }


    bool Architecture::isValid(void) const {
      if (this->arch == triton::arch::ARCH_INVALID)
        return false;
      return true;
    }


    bool Architecture::isFlag(triton::uint32 regId) const {
      if (!this->cpu)
        return false;
      return this->cpu->isFlag(regId);
    }


    bool Architecture::isRegister(triton::uint32 regId) const {
      if (!this->cpu)
        return false;
      return this->cpu->isRegister(regId);
    }


    bool Architecture::isRegisterValid(triton::uint32 regId) const {
      if (!this->cpu)
        return false;
      return this->cpu->isRegisterValid(regId);
    }


    triton::uint32 Architecture::invalidRegister(void) const {
      if (!this->cpu)
        return 0;
      return this->cpu->invalidRegister();
    }


    triton::uint32 Architecture::numberOfRegisters(void) const {
      if (!this->cpu)
        return 0;
      return this->cpu->numberOfRegisters();
    }


    triton::uint32 Architecture::registerSize(void) const {
      if (!this->cpu)
        return 0;
      return this->cpu->registerSize();
    }


    triton::uint32 Architecture::registerBitSize(void) const {
      if (!this->cpu)
        return 0;
      return this->cpu->registerBitSize();
    }


    std::tuple<std::string, triton::uint32, triton::uint32, triton::uint32> Architecture::getRegisterInformation(triton::uint32 reg) const {
      std::tuple<std::string, triton::uint32, triton::uint32, triton::uint32> ret;

      std::get<0>(ret) = "unknown"; /* name           */
      std::get<1>(ret) = 0;         /* highest bit    */
      std::get<2>(ret) = 0;         /* lower bit      */
      std::get<3>(ret) = 0;         /* higest reg id  */

      if (this->cpu)
        ret = this->cpu->getRegisterInformation(reg);

      return ret;
    }


    std::set<triton::arch::Register*> Architecture::getAllRegisters(void) const {
      if (!this->cpu)
        throw triton::exceptions::Architecture("Architecture::getAllRegisters(): You must define an architecture.");
      return this->cpu->getAllRegisters();
    }


    std::set<triton::arch::Register*> Architecture::getParentRegisters(void) const {
      if (!this->cpu)
        throw triton::exceptions::Architecture("Architecture::getParentRegisters(): You must define an architecture.");
      return this->cpu->getParentRegisters();
    }


    void Architecture::disassembly(triton::arch::Instruction& inst) const {
      if (!this->cpu)
        throw triton::exceptions::Architecture("Architecture::disassembly(): You must define an architecture.");
      this->cpu->disassembly(inst);
    }


    bool Architecture::buildSemantics(triton::arch::Instruction& inst) const {
      bool ret = false;

      if (!this->cpu)
        throw triton::exceptions::Architecture("Architecture::buildSemantics(): You must define an architecture.");

      /* Pre IR processing */
      inst.preIRInit();

      /* Processing */
      ret = this->cpu->buildSemantics(inst);

      /* Post IR processing */
      inst.postIRInit();

      return ret;
    }


    triton::uint8 Architecture::getConcreteMemoryValue(triton::uint64 addr) const {
      if (!this->cpu)
        throw triton::exceptions::Architecture("Architecture::getConcreteMemoryValue(): You must define an architecture.");
      return this->cpu->getConcreteMemoryValue(addr);
    }


    triton::uint512 Architecture::getConcreteMemoryValue(const triton::arch::MemoryAccess& mem, bool execCallbacks) const {
      if (!this->cpu)
        throw triton::exceptions::Architecture("Architecture::getConcreteMemoryValue(): You must define an architecture.");
      return this->cpu->getConcreteMemoryValue(mem, execCallbacks);
    }


    std::vector<triton::uint8> Architecture::getConcreteMemoryAreaValue(triton::uint64 baseAddr, triton::usize size, bool execCallbacks) const {
      if (!this->cpu)
        throw triton::exceptions::Architecture("Architecture::getConcreteMemoryAreaValue(): You must define an architecture.");
      return this->cpu->getConcreteMemoryAreaValue(baseAddr, size, execCallbacks);
    }


    triton::uint512 Architecture::getConcreteRegisterValue(const triton::arch::Register& reg, bool execCallbacks) const {
      if (!this->cpu)
        throw triton::exceptions::Architecture("Architecture::getConcreteRegisterValue(): You must define an architecture.");
      return this->cpu->getConcreteRegisterValue(reg, execCallbacks);
    }


    void Architecture::setConcreteMemoryValue(triton::uint64 addr, triton::uint8 value) {
      if (!this->cpu)
        throw triton::exceptions::Architecture("Architecture::setConcreteMemoryValue(): You must define an architecture.");
      this->cpu->setConcreteMemoryValue(addr, value);
    }


    void Architecture::setConcreteMemoryValue(const triton::arch::MemoryAccess& mem) {
      if (!this->cpu)
        throw triton::exceptions::Architecture("Architecture::setConcreteMemoryValue(): You must define an architecture.");
      this->cpu->setConcreteMemoryValue(mem);
    }


    void Architecture::setConcreteMemoryAreaValue(triton::uint64 baseAddr, const std::vector<triton::uint8>& values) {
      if (!this->cpu)
        throw triton::exceptions::Architecture("Architecture::setConcreteMemoryAreaValue(): You must define an architecture.");
      this->cpu->setConcreteMemoryAreaValue(baseAddr, values);
    }


    void Architecture::setConcreteMemoryAreaValue(triton::uint64 baseAddr, const triton::uint8* area, triton::usize size) {
      if (!this->cpu)
        throw triton::exceptions::Architecture("Architecture::setConcreteMemoryAreaValue(): You must define an architecture.");
      this->cpu->setConcreteMemoryAreaValue(baseAddr, area, size);
    }


    void Architecture::setConcreteRegisterValue(const triton::arch::Register& reg) {
      if (!this->cpu)
        throw triton::exceptions::Architecture("Architecture::setConcreteRegisterValue(): You must define an architecture.");
      this->cpu->setConcreteRegisterValue(reg);
    }


    bool Architecture::isMemoryMapped(triton::uint64 baseAddr, triton::usize size) {
      if (!this->cpu)
        throw triton::exceptions::Architecture("Architecture::isMemoryMapped(): You must define an architecture.");
      return this->cpu->isMemoryMapped(baseAddr, size);
    }


    void Architecture::unmapMemory(triton::uint64 baseAddr, triton::usize size) {
      if (!this->cpu)
        throw triton::exceptions::Architecture("Architecture::unmapMemory(): You must define an architecture.");
      this->cpu->unmapMemory(baseAddr, size);
    }

  }; /* arch namespace */
}; /* triton namespace */

