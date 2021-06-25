//-----------------------------------------------
//
//	This file is part of the Siv3D Engine.
//
//	Copyright (c) 2008-2019 Ryo Suzuki
//	Copyright (c) 2016-2019 OpenSiv3D Project
//
//	Licensed under the MIT License.
//
//-----------------------------------------------

# include "CToastNotification.hpp"
# include <future>

namespace s3d
{
	namespace detail
	{
		using s3dRequestNotificationPermissionCallback = void(*)(CToastNotification::NotificationPermission, void* userData);
	
		__attribute__((import_name("s3dRequestNotificationPermission")))
		extern void s3dRequestNotificationPermission(s3dRequestNotificationPermissionCallback callback, void* userData);

		void OnRequestNotificationPermission(CToastNotification::NotificationPermission permission, void* userData)
		{
			auto p = static_cast<std::promise<bool>*>(userData);

			p->set_value(permission == CToastNotification::NotificationPermission::Granted);
			delete p;
		}
	}

	namespace Platform::Web::Notification
	{
		std::future<bool> RequestPermission()
		{
			auto p = new std::promise<bool>();
			auto asyncResult = p->get_future();

			detail::s3dRequestNotificationPermission(&detail::OnRequestNotificationPermission, p);
			return asyncResult;
		}
	}
}