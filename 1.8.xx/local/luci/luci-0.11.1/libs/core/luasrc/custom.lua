
local uci = require("luci.model.uci")

module("luci.custom")

function strip_en()
	return uci.cursor():get_bool("luci", "custom", "strip")
end


