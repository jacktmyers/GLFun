local dap = require("dap")
dap.adapters.cppdbg = {
	id = "cppdbg",
	type = "executable",
	command = "/home/jack/.config/cpptools/extension/debugAdapters/bin/OpenDebugAD7"
}
dap.configurations.cpp = {
  {
    name = "Launch Modern",
    type = "cppdbg",
    request = "launch",
    program = function()
			vim.cmd("silent! !rm -rf ./Modern")
			vim.cmd("!make")
      return "./Modern"
    end,
    cwd = '${workspaceFolder}',
    stopAtEntry = true,
	}
}
