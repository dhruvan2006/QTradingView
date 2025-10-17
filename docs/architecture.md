# Architecture

QTradingView is designed as a small, modular charting library with a clear separation between layout, rendering and data. The main concepts are:

- Chart — top-level container that manages panes, input events, layout and a shared viewport. It owns the main render loop.
- Pane — a rectangular area inside a `Chart` that has its own Y-scale and renders one or more `Series` (candles, lines, bars).
- ViewPort — shared viewport that tracks visible indices, bar width, and translates from data indices to pixel coordinates.
- Series — pluggable renderers that draw data to a `QPainter` using the pane's scale and the viewport for X-axis mapping.
- Scales & Renderers — scale implementations (linear/log) implement the `IScale` interface. Renderers such as `AxisRenderer`, `GridRenderer` and `CrosshairRenderer` draw overlays and axis labels.

This separation enables partial redraws (only panes that changed are repainted), easy addition of new series types, and different scale strategies per pane.

For a class-level view, see the Doxygen-generated API under `docs/api/` and the diagram in the repository README.

## Typical flow

1. User creates a `Chart` and adds one or more `Pane` objects.
2. The user attaches `Series` instances to panes and provides data containers.
3. `ViewPort` determines which indices are visible; the chart requests each pane to render using that viewport.
4. Renderers convert data points to pixel coordinates using the pane's `IScale` implementation and draw primitives with `QPainter`.

## Extending the library

- To add a new series type, inherit from `QTradingView::Series` and implement the `render` method.
- To add a new scale, implement `IScale` and plug it into a `Pane` with `setScale`.

Check the API docs (`docs/api/`) for interfaces, header-level documentation and examples in `docs/examples/`.

