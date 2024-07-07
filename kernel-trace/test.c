#include <stdio.h>




int main(){ 
    struct nvme_id_ctrl {
	char			sn[20];
	char			mn[40];
	char			fr[8];
};
    printf("lenglg: %d", sizeof(struct nvme_id_ctrl));
}
