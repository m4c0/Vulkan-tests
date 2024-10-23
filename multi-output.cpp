#pragma leco app
#pragma leco add_shader "multi-output.frag"
#pragma leco add_shader "multi-output.vert"

import casein;
import vee;
import voo;

struct thread : public voo::casein_thread {
  void run() override {
    main_loop("test", [&](auto & dq, auto & sw) {
      auto pl = vee::create_pipeline_layout();
      voo::one_quad_render oqr { "multi-output", &dq, *pl };

      ots_loop(dq, sw, [&](auto cb) {
        oqr.run(cb, sw.extent());
      });
    });
  }
} i;
