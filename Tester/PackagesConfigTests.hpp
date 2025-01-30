#pragma once

namespace PackagesConfigTests {
	void WhenUnchanged_TakesCurrent();
	void WhenUnchanged_AndInvalid_TakesCurrent();
	
	void WhenCurrentUpgraded_TakesCurrent();
	void WhenCurrentUpgraded_AndOtherDowngraded_Fails();
	void WhenCurrentUpgraded_AndOtherDeleted_TakesOther();
	void WhenCurrentUpgraded_AndOtherInvalid_Fails();
	
	void WhenCurrentDowngraded_TakesCurrent();
	void WhenCurrentDowngraded_AndOtherUpgraded_Fails();
	void WhenCurrentDowngraded_AndOtherDeleted_TakesOther();
	void WhenCurrentDowngraded_AndOtherInvalid_Fails();
	
	void WhenCurrentDeleted_AndOtherUpgraded_TakesCurrent();
	void WhenCurrentDeleted_AndOtherDowngraded_TakesCurrent();
	
	void WhenCurrentInvalid_AndOtherUpgraded_Fails();
	void WhenCurrentInvalid_AndOtherDowngraded_Fails();
}
