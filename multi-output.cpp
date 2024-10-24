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
      voo::swapchain_and_stuff sw { dq };

      // TODO: add more attachments
      auto rp = vee::create_render_pass(dq.physical_device(), dq.surface());

      auto pl = vee::create_pipeline_layout();
      voo::one_quad_render oqr { "multi-output", dq.physical_device(), *rp, *pl };

      ots_loop(dq, sw, [&](auto cb) {
        oqr.run(cb, sw.extent());
      });
    }
  }
} i;
