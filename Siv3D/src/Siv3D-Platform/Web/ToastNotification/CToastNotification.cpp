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

# include <Siv3D/EngineLog.hpp>
# include "CToastNotification.hpp"

namespace s3d
{
	namespace detail
	{
		using s3dCreateNotificationCallback = void(*)(CToastNotification::Notification::IDType idx, void* userData);
		using s3dNotificationEventCallback = void(*)(CToastNotification::Notification::IDType idx, ToastNotificationState state, void* userData);

		__attribute__((import_name("s3dInitNotification")))
		extern void s3dInitNotification();

		__attribute__((import_name("s3dCreateNotification")))
		extern CToastNotification::Notification::IDType s3dCreateNotification(const char* title, const char* body, size_t actionsNum, const char* const* actions, s3dCreateNotificationCallback callback, void* userData);

		__attribute__((import_name("s3dRegisterNotificationCallback")))
		extern void s3dRegisterNotificationCallback(CToastNotification::Notification::IDType idx, s3dNotificationEventCallback callback, void* userData);

		__attribute__((import_name("s3dCloseNotification")))
		extern void s3dCloseNotification(CToastNotification::Notification::IDType idx);

		__attribute__((import_name("s3dQueryNotificationAvailability")))
		extern bool s3dQueryNotificationAvailability();

	}

	CToastNotification::CToastNotification() {}

	CToastNotification::~CToastNotification()
	{
		clear();

		LOG_TRACE(U"CToastNotification::~CToastNotification()");
	}

	void CToastNotification::init()
	{
		LOG_TRACE(U"CToastNotification::init()");

		detail::s3dInitNotification();

		LOG_INFO(U"ℹ️ CToastNotification initialized");
	}

	bool CToastNotification::isAvailable() const
	{
		return detail::s3dQueryNotificationAvailability();
	}

	NotificationID CToastNotification::show(const ToastNotificationProperty& item)
	{
		const auto& title = item.title.toUTF8();
		const auto& message = item.message.toUTF8();

		const auto& actions = item.actions.map([](const auto& item) { return item.toUTF8(); });
		const auto& actionsCptr = actions.map([](const auto& item) { return item.c_str(); });

		return detail::s3dCreateNotification(
			title.c_str(), message.c_str(), 
			actions.size(), actionsCptr.data(), 
			&CToastNotification::OnCreateNotification, this);
	}

	ToastNotificationState CToastNotification::getState(NotificationID idx)
	{
		auto it = m_notifications.find(idx);

		if (it == m_notifications.end())
		{
			return(ToastNotificationState::None);
		}

		return it->second.state;
	}

	Optional<size_t> CToastNotification::getAction(NotificationID)
	{
		// [Siv3D ToDo]
		return(none);
	}

	void CToastNotification::hide(NotificationID idx)
	{
		detail::s3dCloseNotification(idx);
	}

	void CToastNotification::clear()
	{
		for (auto [ key, state ] : m_notifications)
		{
			detail::s3dCloseNotification(key);
		}

		m_notifications.clear();
	}

	void CToastNotification::onStateUpdate(size_t idx, ToastNotificationState state, const Optional<int32>&)
	{
		auto it = m_notifications.find(idx);

		if (it == m_notifications.end())
		{
			return;
		}

		it.value().state = state;
	}

	void CToastNotification::OnStateUpdate(Notification::IDType idx, ToastNotificationState state, void* userData)
	{
		auto& notification = *static_cast<CToastNotification*>(userData);

		notification.onStateUpdate(static_cast<size_t>(idx), state, none);
	}

	void CToastNotification::OnCreateNotification(Notification::IDType idx, void* userData)
	{
		auto& notification = *static_cast<CToastNotification*>(userData);

		notification.m_notifications[idx] = Notification{};

		detail::s3dRegisterNotificationCallback(idx, &CToastNotification::OnStateUpdate, &notification);
	}
}
