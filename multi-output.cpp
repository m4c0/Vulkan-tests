#pragma leco app
#pragma leco add_shader "multi-output.frag"
#pragma leco add_shader "multi-output.vert"

import casein;
import vee;
import voo;

struct thread : public voo::casein_thread {
  void run() override {
    voo::device_and_queue dq { "test" };
    while (!interrupted()) {
      auto rp = vee::create_render_pass({{
        vee::create_colour_attachment(dq.physical_device(), dq.surface()),
        vee::create_colour_attachment(),
      }});
      auto pd = dq.physical_device();
      auto s = dq.surface();
      auto q = dq.queue();

      voo::offscreen::colour_buffer extra { pd, voo::extent_of(pd, s) };
      voo::swapchain_and_stuff sw { dq, *rp, {{ extra.image_view() }} };

      // TODO: add more attachments

      auto pl = vee::create_pipeline_layout();
      voo::one_quad_render oqr { "multi-output", dq.physical_device(), *rp, *pl };

      extent_loop(q, sw, [&] {
        sw.queue_one_time_submit(q, [&](auto pcb) {
          voo::cmd_render_pass scb {{
            .command_buffer = *pcb,
            .render_pass = *rp,
            .framebuffer = sw.framebuffer(),
            .extent = sw.extent(),
            .clear_colours = {{ {}, {} }},
          }};
          oqr.run(*scb, sw.extent());
        });
      });
    }
  }
} i;
