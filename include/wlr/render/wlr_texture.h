/*
 * This an unstable interface of wlroots. No guarantees are made regarding the
 * future consistency of this API.
 */
#ifndef WLR_USE_UNSTABLE
#error "Add -DWLR_USE_UNSTABLE to enable unstable wlroots features"
#endif

#ifndef WLR_RENDER_WLR_TEXTURE_H
#define WLR_RENDER_WLR_TEXTURE_H

#include <pixman.h>
#include <stdint.h>
#include <wayland-server-core.h>
#include <wlr/render/dmabuf.h>

struct wlr_buffer;
struct wlr_renderer;
struct wlr_texture_impl;

struct wlr_texture {
	const struct wlr_texture_impl *impl;
	uint32_t width, height;

	struct wlr_renderer *renderer;
};

/**
 * Create a new texture from raw pixel data. `stride` is in bytes. The returned
 * texture is mutable.
 */
struct wlr_texture *wlr_texture_from_pixels(struct wlr_renderer *renderer,
	uint32_t fmt, uint32_t stride, uint32_t width, uint32_t height,
	const void *data);

/**
 * Create a new texture from a DMA-BUF. The returned texture is immutable.
 */
struct wlr_texture *wlr_texture_from_dmabuf(struct wlr_renderer *renderer,
	struct wlr_dmabuf_attributes *attribs);

/**
  * Update a texture with a struct wlr_buffer's contents.
  *
  * The update might be rejected (in case the texture is immutable, the buffer
  * has an unsupported type/format, etc), so callers must be prepared to fall
  * back to re-creating the texture from scratch via wlr_texture_from_buffer().
  *
  * The damage can be used by the renderer as an optimization: only the supplied
  * region needs to be updated.
  */
bool wlr_texture_update_from_buffer(struct wlr_texture *texture,
	struct wlr_buffer *buffer, const pixman_region32_t *damage);

/**
 * Destroys the texture.
 */
void wlr_texture_destroy(struct wlr_texture *texture);

/**
 * Create a new texture from a buffer.
 */
struct wlr_texture *wlr_texture_from_buffer(struct wlr_renderer *renderer,
	struct wlr_buffer *buffer);

/**
 * Create a new buffer texture from a EGLSTREAM_WL resource.
 *
 * Should not be called in a rendering block like renderer_begin()/end() or
 * between attaching a renderer to an output and committing it.
 */
struct wlr_buffer;
struct wlr_buffer *wlr_buffer_from_wl_eglstream(struct wlr_renderer *renderer,
	struct wl_resource *resource, struct wl_array *attribs);

#endif
