#include "../limine.h"
#include <stdint.h>
#include <stdbool.h>

static volatile struct limine_smp_request smp_request = {
	.id = LIMINE_SMP_REQUEST,
	.revision = 0,
	.flags = 0,
};

typedef uint64_t pagemap_t;

typedef struct core {
	/* Core number */
	uint64_t core_number;

	/* Local APIC Id */
	uint32_t lapic_id;

	/* Current core pagemap */
	pagemap_t *pagemap;

	/* If our core is the one that ran start */
	bool bsp;
} core_t;

uint32_t bsp_lapic_id = 0;
uint64_t coreCount = 0;
static uint64_t initialized = 0;
core_t *cpu_core_local = NULL;

void smp_init()
{
    struct limine_smp_response *smp_response = smp_request.response;
	struct limine_smp_info **cpu_cores = smp_response->cpus;

	coreCount = smp_response->cpu_count;

    cpu_core_local = malloc(sizeof(core_t) * coreCount);

    bsp_lapic_id = smp_response->bsp_lapic_id;

	for(uint64_t i = 0; i < coreCount; i++) {
		struct limine_smp_info* core = cpu_cores[i];

		/* Get the core_t from cpu_core_local */
		core_t* current = &cpu_core_local[i];
		current->core_number = i;

		core->extra_argument = (uint64_t)current;
		
		/* If core is bsp then goto the function */
		if(core->lapic_id != smp_response->bsp_lapic_id) {
			core->goto_address = core_start; /* Jump to core start */
		} else {
			current->bsp = true;
			core_start(core);
		}
	}

	/* Safe guard, core 0 does not jump to core_start as it is already running */
	while(initialized != (coreCount - 1)) {
		asm ("pause");
	}
}