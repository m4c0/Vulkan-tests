#pragma leco app
#pragma leco add_shader "multi-output.frag"
#pragma leco add_shader "multi-output.vert"

import casein;
import silog;
import vee;
import voo;

struct upc {
  float aspect;
};

struct thread : public voo::casein_thread {
  void run() override {
    voo::device_and_queue dq { "test" };
    auto pd = dq.physical_device();
    auto s = dq.surface();
    auto q = dq.queue();

    constexpr const auto fmt = vee::image_format_rgba_unorm;

    while (!interrupted()) {
      auto rp = vee::create_render_pass({{
        vee::create_colour_attachment(pd, s),
        vee::create_colour_attachment(fmt),
      }});
      voo::offscreen::colour_buffer cbuf { pd, voo::extent_of(pd, s), fmt };
      voo::offscreen::host_buffer hbuf { pd, { 1, 1 } };

      voo::swapchain_and_stuff sw { dq, *rp, {{ cbuf.image_view() }} };

      auto pl = vee::create_pipeline_layout({
        vee::vert_frag_push_constant_range<upc>()
      });
      voo::one_quad_render oqr { "multi-output", pd, *rp, *pl, {
        vee::colour_blend_classic(), vee::colour_blend_none()
      } };

      extent_loop(q, sw, [&] {
        sw.queue_one_time_submit(q, [&](auto pcb) {
          upc pc { .aspect = sw.aspect() };

          voo::cmd_render_pass scb {vee::render_pass_begin {
            .command_buffer = *pcb,
            .render_pass = *rp,
            .framebuffer = sw.framebuffer(),
            .extent = sw.extent(),
            .clear_colours {
              vee::clear_colour(0, 0, 0, 0),
              vee::clear_colour(0, 0, 0, 0),
            },
          }};
          vee::cmd_push_vert_frag_constants(*scb, *pl, &pc);
          oqr.run(*scb, sw.extent());
          cbuf.cmd_copy_to_host(*scb, {}, { 1, 1 }, hbuf.buffer());

          static int count = 0;
          if (count < 5) {
            auto mem = hbuf.map();
            silog::log(silog::debug, "%08x", *static_cast<unsigned *>(*mem));
            count++;
          }
          });
      });
    }
  }
} i;
