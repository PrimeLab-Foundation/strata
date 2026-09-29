"""Framework adapters: one module per framework, each filling that framework's JSON hook.

``strata.integrations.flask``, ``.django``, ``.aiohttp``, ``.falcon``,
``.fastapi``, ``.pydantic`` and ``.structlog`` (docs/context/api.md, Framework
adapters). Nothing here is imported by ``import strata`` or by importing this
package; a framework is imported only when its adapter module is.
"""
