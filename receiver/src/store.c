#include "config.h"
#include "proto.h"
#include "main.h"
#include "store.h"

#define TAGS_LIST_LENGTH 10

typedef struct TagSlot {

	ProtoProxyTag tag;
	struct TagSlot *next;
	struct TagSlot *prev;

} TagSlot_t;

TagSlot_t  g_store[TAGS_LIST_LENGTH];
TagSlot_t *g_head;

uint32_t rec;

void store_init(uint32_t oid) {
	memset((uint8_t*) &g_store, 0, sizeof(g_store));
	g_head = 0;
	rec = oid;
}

void store_get(ProtoEnvelope *env) {

	for (uint8_t i = 0; i < PROXY_TAGS_LENGTH; i++) {
		if (g_head) {

			memcpy(&env->data.proxy.tags[i], &g_head->tag, sizeof(ProtoProxyTag));

			g_head = g_head->next;
			if (g_head) g_head->prev = 0;

		} else {

			memset((uint8_t*) &env->data.proxy.tags[i], 0, sizeof(ProtoProxyTag));

		}
	}
}

void store_save(const ProtoEnvelope *env, const int8_t rssi) {
	TagSlot_t *slot = 0;

	// Find slot by oid or take free

	for (uint8_t i = 0; i < TAGS_LIST_LENGTH; i++) {

		if (g_store[i].tag.oid == env->oid) {
			slot = &g_store[i];
			break;
		}

		if (!slot) {
			if (g_store[i].tag.oid == 0) {
				slot = &g_store[i];
			}
		}

	}

	// If slot still not found - take the last by time

	if (!slot) {
		slot = g_head;
		while (slot->next != 0) {
			slot = slot->next;
		}
	}

	// Update slot
	slot->tag.rec  = rec;
	slot->tag.oid  = env->oid;
	slot->tag.batt = env->batt;
	slot->tag.rssi = rssi;
	slot->tag.seq  = env->seq;
	slot->tag.temp = env->temp;

	// Move slot to the head

	if (slot != g_head) {

		if (slot->prev)	slot->prev->next = slot->next;
		if (slot->next)	slot->next->prev = slot->prev;
		if (g_head) g_head->prev = slot;

		slot->prev = 0;
		slot->next = g_head;
		g_head = slot;
	}

}
