#pragma leco app
#pragma leco add_shader "multi-output.frag"
#pragma leco add_shader "multi-output.vert"

import casein;
import vee;
import voo;

struct thread : public voo::casein_thread {
  void run() override {
    voo::device_and_queue dq { "test" };
    auto pd = dq.physical_device();
    auto s = dq.surface();

    while (!interrupted()) {
      auto rp = vee::create_render_pass({{
        vee::create_colour_attachment(pd, s),
        vee::create_colour_attachment(),
      }});
      voo::offscreen::colour_buffer cb { pd, voo::extent_of(pd, s) };

      voo::swapchain_and_stuff sw { dq, *rp, {{ cb.image_view() }} };

      auto pl = vee::create_pipeline_layout();
      voo::one_quad_render oqr { "multi-output", pd, *rp, *pl };

      ots_loop(dq, sw, [&](auto cb) {
        oqr.run(cb, sw.extent());
      });
    }
  }
} i;
